#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

#define MAX_LENGTH 1024
#define MAX_ARGS 64
#define MAX_PROCS 10

typedef enum { false = 0, true = 1} Bool;
typedef struct {
	int isDead;
	pid_t pid;
	struct ProcessTableEntry* next;
} ProcessTableEntry;

char* inputFileName = "\0";
char* outputFileName = "\0";


int background = 0;
int resetLine = 0;
pid_t pid = 0;
pid_t spid = 0;



/* Poor way of resetting line in the case of the shell encountering ^C
 * Must be called before and after readLine(line) for the two cases:
 * 	1. ^C was entered on the blank line, we just want to send "\n" to parse which causes a reprompt
 * 	2. ^C was entered while a child was in execution, we need to reset the flag before reading in a new line
 */
void reset(char* line){
	if(resetLine){
		resetLine = 0;
		strcpy(line, "\n");
	}
}

void readLine(char* line){
	printf("mysh %c ", '%');
	reset(line);
	fgets(line, MAX_LENGTH, stdin);
	reset(line);
}

int lookForPipe(char* line, char* part1, char* part2){
	int pipeFlag = 0;
	char copy[strlen(line)];

	strcpy(copy,line);
	strcpy(part1,strtok(line, "|"));

	if(strlen(part1) != strlen(copy)){
		strcpy(part2, strtok(NULL,"|"));
		pipeFlag = 1;
	} else{
		strcpy(line,copy);
	}
	return pipeFlag;
}

void resume(){
	if(spid > 0){
		kill(spid, SIGCONT);
		spid = 0;
	}
}

void reprompt(char* line, char** args){
	readLine(line);
	parseLine(line, args);
}

/* returns the number of parsed tokens added to the args array */
int parseLine(char* line, char** args){
	int pos = 0, resumeFlag = 0;
	char *token = NULL;
	char delims[] = {" \n"};
	outputFileName = "\0", inputFileName = "\0";

	//strcmp returns 0 if the values are equal
	if((strcmp(line,"\n") != 0)){ //don't parse if input is just "\n"
		token = strtok(line, delims);
		/* These conditions we only care about if they are the first token */
		if(!strcmp(token, "exit")){ exit(EXIT_SUCCESS); }
		else if(!strcmp(token, "resume")){
			resumeFlag = 1;
			resume();
		}
		else if(!strcmp(token, "cd")){
			args[pos] = strdup(token);
			token = strtok(NULL, delims);
			chdir(token);
			pos++;
		} else{
			/* These conditions can occur anywhere in the input string */
			while(token != '\0'){
				//echo a > a.txt
				if(!strcmp(token, ">")){ //if there is file output redirection
					token = strtok(NULL, delims);
					outputFileName = token;
					token = strtok(NULL, delims);
				}
				//echo a >a.txt
				else if(!strncmp(token, ">", 1)){
					outputFileName = token+1;
					token = strtok(NULL, delims);
				}
				else if(!strcmp(token, "<")){ //if there is file input redirection
					token = strtok(NULL, delims);
					inputFileName = token;
					token = strtok(NULL, delims);
				}
				else if(!strncmp(token, "<", 1)){
					inputFileName = token+1;
					token = strtok(NULL, delims);
				}
				else {
					args[pos] = strdup(token);
					token = strtok(NULL, delims);
					//printf("args[%d]: %s\n", pos, args[pos]);
					pos++;
				}
			}
		}
		//Assuming & is always the last entered argument, if it is present, set the flag, and remove from the argument list
		if(!resumeFlag){
			if(!strcmp(args[pos-1], "&")){
				background = 1;
				args[pos-1] = '\0';
			} else{ //Else, reset the flag
				background = 0;
			}
		}
	} else{
		//If the input was just "\n", prompt for another line, without processing
		reprompt(line, args);
	}

	return pos;
}

void redirect(){
	int fd_in, fd_out;
	if(strcmp(inputFileName, "\0")){
		close(STDIN_FILENO);
		fd_in = open(inputFileName, O_RDONLY, 0);
	}
	if(strcmp(outputFileName, "\0")){
		close(STDOUT_FILENO);
		fd_out = open(outputFileName, O_TRUNC | O_WRONLY | O_CREAT ,0644);
	}

}

void execute(char **args){
	if(execvp(*args, args) < 0){
		printf("**** ERROR: exec failed\n");
	}
	exit(EXIT_FAILURE);
}

void sig_interrupt(int signo){
	signal(SIGINT, sig_interrupt);
	printf("\n");
	if(pid > 0){
		kill(pid, SIGINT);
		pid = 0;
	}
	resetLine = 1;
}

void sig_stop(int signo){
	signal(SIGTSTP, sig_stop);
	printf("\n");
	if(pid > 0){
		kill(pid, SIGTSTP);
		spid = pid;
		pid = 0;
	}
	resetLine = 1;
}


/*Basic functions to keep track of a LinkedList of ProcessTableEntrys*/

void append(ProcessTableEntry* head, int pid)
{
    ProcessTableEntry *temp,*pte;
    temp = head;
    pte = (struct ProcessTableEntry *)malloc(sizeof(ProcessTableEntry));
    pte->isDead = 0;
    pte->pid=pid;
    while(temp->next != NULL){
    	temp=temp->next;
    }
    temp->next = pte;
    temp=pte;
    pte->next=NULL;
}

//Adding a Node at the end of the list

void addEnd(ProcessTableEntry* head, int pid){
	ProcessTableEntry* temp1;
	ProcessTableEntry* temp2;
	temp1 = (ProcessTableEntry*)malloc(sizeof(ProcessTableEntry));
	temp1->isDead = 0;
	temp1->pid = pid;

	temp2 = head;

	if(head == NULL){
		head = temp1;
		head->next = NULL;
	}
	else{
		while(temp2->next != NULL){
			temp2 = temp2->next;
		}
		temp2->next = temp1;
		temp1->next = NULL;
	}
}

int delete(ProcessTableEntry *head, int pid)
{
    ProcessTableEntry *temp, *prev;
    temp=head;
    while(temp != NULL){
		if(temp->pid == pid){
			if(temp==head){
				head = temp->next;
				free(temp);
				return 1;
			}
			else {
				prev->next = temp->next;
				free(temp);
				return 1;
			}
		}
		else{
			prev = temp;
			temp = temp->next;
		}
    }
    return 0;
}

// Counting number of elements in the List
int length(ProcessTableEntry* head){
  ProcessTableEntry *curr;
  int count=0;
  curr=head;

  while(curr != NULL){
     curr = curr->next;
     count++;
  }

  return(count);
}

ProcessTableEntry* find(ProcessTableEntry* head, int pid){
	ProcessTableEntry* temp = head;
	while(temp->pid != pid){
		temp = temp->next;
	}
	return temp;
}

ProcessTableEntry* init(){
	ProcessTableEntry* temp = malloc(sizeof(ProcessTableEntry));
	temp->isDead = 0;
	temp->pid = getpid();
	temp->next = NULL;
	return temp;
}


int main(int argc, char *argv[]){
	char line[MAX_LENGTH]; 	// input line
	char part1[MAX_LENGTH];	// input line part 1 for pipes
	char part2[MAX_LENGTH];	// input line part 2 for pipes
	char *args[MAX_ARGS];	// command line arguments
	char *args2[MAX_ARGS];  // command line arguments for pipes
	int status,pipeFlag,count=0;
	int pipefd[2];
	pid_t cid,tid;

	ProcessTableEntry* parent = init();
	ProcessTableEntry* pte = parent;


	memset(&args[0], 0, sizeof(args));
	memset(&args2[0], 0, sizeof(args2));

	signal(SIGINT, sig_interrupt);
	signal(SIGTSTP, sig_stop);

	while(1){
		/* Linked list process table stuff (DOESN'T WORK) */
//		printf("length = %d\n",length(parent));
//		while(pte->next != NULL){
//			pte = pte->next;
//			//printf("[%d] = %d\n", ++count, pte->pid);
//			tid = waitpid(pte->pid, &status, WNOHANG);
//			if(tid == 0) delete(pte, pte->pid);
//		}
//		count = 0;
//		pte = parent;

		/* Read in the command line */
		readLine(line);

		/* Check for pipes, parse accordingly */
		pipeFlag = lookForPipe(line, part1, part2);
		if(pipeFlag){
			parseLine(part1, args);
			parseLine(part2, args2);
			pipe(pipefd);
		}
		else{
			parseLine(line, args);
		}
		/* Fork the processes */
		pid = fork();
		if(pid < 0) printf("Fork error \n");
		else if(pid == 0){ //child
			/* If there was a pipe, execute accordingly */
			if(pipeFlag){
				int ppid = fork();
				if(ppid < 0) printf("Fork error \n");
				else if (ppid == 0){ //child
					dup2(pipefd[0], 0);
					close(pipefd[1]);
					execute(args2);
				} else{ //parent
					dup2(pipefd[1],1);
					close(pipefd[0]);
					execute(args);
				}
			}
			/* Else, execute the child as normal */
			else{
				redirect();
				execute(args);
			}
		}
		else{ //parent
			/* If the child is not executed in the background, wait for it */
			if(background){
				addEnd(parent, pid);
			} else{
				do {
					cid = waitpid(pid, &status, WUNTRACED | WCONTINUED);
					if (cid < 0) {
						perror("waitpid error");
						printf("[ Program returned exit code %d ]\n", status);
					}
				} while (!WIFEXITED(status) && !WIFSIGNALED(status));
			}
		}

		/* Reset the args arrays */
		memset(&args[0], 0, sizeof(args));
		memset(&args2[0], 0, sizeof(args2));
	}

	return 0;
}

/*
 * Scheduler.c
 *
 *  Created on: Apr 13, 2014
 *      Author: Anders Van Winkle
 *
 *
 *  From assumptions.txt
 *
 *  Assumptions:
 *	- If a process finishes on cycle n, the next process will start on cycle n+1
 *		- That includes context switches, the CS will begin at n+1
 *		- i.e. half CS = 2, process terminates on cycle 20, new process could start at 21
 *			- if that process needs to be restored, 21 and 22 are the CS and the process starts on 23
 *	- If a process finishes, there is no (half) context switch to back up its registers
 *		- There is, however, a (half) context switch to restore a process that has previously been on the CPU, even after a terminated process
 *	- Every time a process is removed from running for IO, there is a halfCS and when they return, another halfCS
 *		- Even if there is only 1 process remaining that must do multiple CPU/IO bursts. It will do a full CS every time.
 */

#include <stdio.h>
#include <string.h>
#include "Scheduler.h"
#include "Dispatcher.h"

//Globals
int preemptiveCount = 0;

//Allocates and creates a new PCB struct
PCB* newPCB(int pid, int aTime, int bursts, int* CPUbursts, int* IObursts){
	int i;
	PCB * pcb;
	pcb = malloc(sizeof(PCB));

	pcb->pid = pid;
	pcb->aTime = aTime;
	pcb->fTime = 0;
	pcb->rTime = 0;
	for(i = 0; i<MAX_BURSTS; i++) pcb->CPUbursts[i] = CPUbursts[i];
	for(i = 0; i<MAX_BURSTS-1; i++) pcb->IObursts[i] = IObursts[i];
	pcb->currCPUburst = 0;
	pcb->currCPUburstRemaining = CPUbursts[0];
	pcb->currIOburst = 0;
	pcb->currIOburstRemaining = IObursts[0];
	pcb->numBursts = bursts;
	pcb->currNumBurstsRemaining = bursts;

	pcb->mustBeRestored = 0;

	pcb->next = NULL;
	pcb->prev = NULL;
	return pcb;
}

//Insert at head
void insert(Queue *q, PCB *item){
	item->next = NULL;
	item->prev = NULL;
	if(q->head == NULL){
		q->head = item;
		q->tail = item;
	}
	else{
		q->head->prev = item;
		item->next = q->head;
		q->head = item;
	}
}

//Remove PCB from Queue based on pid
PCB* pidRemove(Queue * q, int pid){
    PCB *currP, *prevP = NULL;
    for (currP = q->head; currP != NULL; prevP = currP, currP = currP->next) {
	    if (currP->pid == pid) {
	    	if (prevP == NULL) {
	    		q->head = currP->next;
	    	} else {
	    		prevP->next = currP->next;
	    	}
	    return currP;
	    }
    }
    return NULL; //NOT FOUND
}

//Remove from tail
PCB* tailRemove(Queue *q){
	PCB* obj = malloc(sizeof(PCB));
	if (q->head == NULL){
		return NULL;
	}
	else if(q->head == q->tail){
		obj = q->head;
		q->head = NULL;
		q->tail = NULL;
	}
	else{
		obj = q->tail;
		q->tail->prev->next = NULL;
		q->tail = q->tail->prev;
		obj->prev = NULL;
		obj->next = NULL;
	}
	return obj;
}

//Prints the contents of the given Queue
void printContents(Queue * q){
	PCB* temp = q->head;
	int i = 0;
	printf("At time %d the queue contains ", getTime());
	while(temp != NULL){
		printf("P%d ", temp->pid);
		temp = temp->next;
	}
	printf("\n");
}

//Returns the number of elements in the given Queue
int size(Queue * q){
	PCB* temp = q->head;
	int i = 0;
	while(temp != NULL){
		temp = temp->next;
		i++;
	}
	return i;
}

//Parse the file line into an integer array
void parseLine(int* args, char* line){
	char *token = NULL;
	char delims[] = {" \n"};
	int i = 0;

	token = strtok(line, delims);
	while(token != '\0'){
		args[i++] = atoi(token);
		token = strtok(NULL, delims);
	}
}

//Process the given process's metrics upon termination
void processTerminationMetrics(PCB* pcb){
	int i, totalBurstTime = 0, totalIOTime = 0;
	int turnAroundTime = pcb->fTime - pcb->aTime; //Turn around time = finish time - arrival time
	for(i = 0; i<pcb->numBursts; i++){
		totalBurstTime += pcb->CPUbursts[i];
		if(i < (pcb->numBursts-1)) totalIOTime += pcb->IObursts[i];
	}
	int waitTime = turnAroundTime - totalBurstTime;
	int responseTime = pcb->rTime - pcb->aTime; //Response time = first time on CPU - arrival time

	Metrics * metrics = getMetrics();
	metrics->burstTotal += totalBurstTime;
	metrics->sumTAT += turnAroundTime;
	if(turnAroundTime > metrics->maxTAT) metrics->maxTAT = turnAroundTime;
	metrics->sumWT += waitTime;
	if(waitTime > metrics->maxWT) metrics->maxWT = waitTime;
	metrics->sumRT += responseTime;
	if(responseTime > metrics->maxRT) metrics->maxRT = responseTime;
	metrics->sumIO += totalIOTime;
	if(totalIOTime > metrics->maxIO) metrics->maxIO = totalIOTime;

	printf("Process \"P%d\" terminated at time %d: TAT=%d, Wait Time=%d, I/O Wait=%d, Response Time=%d.\n",
			pcb->pid, pcb->fTime, turnAroundTime, waitTime, totalIOTime, responseTime);
}

//Non-preemptive scheduling algorithm
void nonpreemptive(Queue* q){
	if(!idle()){
		CPU * cpu = getCPU();
		if(cpu->pcb->currCPUburstRemaining > 0){ //Decrement current CPU burst
			int x = cpu->pcb->currCPUburstRemaining;
			x--;
			cpu->pcb->currCPUburstRemaining = x;
		}
		else if (cpu->pcb->currCPUburstRemaining == 0 && (cpu->pcb->currNumBurstsRemaining-1) != 0){ //Take process off CPU for IO burst
			cpu->pcb->currIOburstRemaining = cpu->pcb->IObursts[cpu->pcb->currIOburst];
			printf("Process \"P%d\" is waiting at time %d for %d cycles.\n",cpu->pcb->pid, cpu->time, cpu->pcb->currIOburstRemaining);

			int z = cpu->pcb->currIOburstRemaining;
			z++;
			cpu->pcb->currIOburstRemaining = z;
			insert(cpu->waitList, cpu->pcb);
			cpu->pcb->currIOburst++;

			//Handle CPU bursts
			cpu->pcb->currNumBurstsRemaining--;
			cpu->pcb->currCPUburst++;
			cpu->pcb->currCPUburstRemaining = cpu->pcb->CPUbursts[cpu->pcb->currCPUburst];

			//Perform half CS to remove process from CPU
			halfCS();
		}
		else if (cpu->pcb->currCPUburstRemaining == 0 && (cpu->pcb->currNumBurstsRemaining-1) == 0){
			cpu->pcb->fTime = cpu->time;
			processTerminationMetrics(cpu->pcb);
			cpu->pcb = NULL;
		}
	}
}

//Preemptive scheduling algorithm
void preemptive(Queue* q){
	if(!idle()){
		CPU * cpu = getCPU();
		int burstRemaining = cpu->pcb->currCPUburstRemaining;
		if(burstRemaining - cpu->quantum > 0){	//This process won't finish its burst in the given quantum
			preemptiveCount++;
			if(preemptiveCount > cpu->quantum){ //Preempt the process one its has been on the CPU for the given quantum
				preemptiveCount = 0;
				preempt();
			}
		}
		else if(burstRemaining > 0){
			int x = cpu->pcb->currCPUburstRemaining;
			x--;
			cpu->pcb->currCPUburstRemaining = x;
		}
		else if (burstRemaining == 0 && (cpu->pcb->currNumBurstsRemaining-1) != 0){	//Process needs to wait on an IO
			cpu->pcb->currIOburstRemaining = cpu->pcb->IObursts[cpu->pcb->currIOburst];
			printf("Process \"P%d\" is waiting at time %d for %d cycles.\n",cpu->pcb->pid, cpu->time, cpu->pcb->currIOburstRemaining);

			//Handle IO bursts
			int z = cpu->pcb->currIOburstRemaining;
			z++;
			cpu->pcb->currIOburstRemaining = z;
			insert(cpu->waitList, cpu->pcb);
			cpu->pcb->currIOburst++;

			//Handle CPU bursts
			cpu->pcb->currNumBurstsRemaining--;
			cpu->pcb->currCPUburst++;
			cpu->pcb->currCPUburstRemaining = cpu->pcb->CPUbursts[cpu->pcb->currCPUburst];

			//Perform half CS to remove process from CPU
			halfCS();
		}
		else if (burstRemaining == 0 && (cpu->pcb->currNumBurstsRemaining-1) == 0){ //If there is only 1 more CPUburst, there are no more IO bursts, terminate the process
			cpu->pcb->fTime = cpu->time;
			processTerminationMetrics(cpu->pcb);
			cpu->pcb = NULL;
		}
	}
}

//Execute the given algorithm
void execAlg(char* alg, Queue* q){
	PCB* pcb = NULL;
	if(idle()) pcb = tailRemove(q);
	if(pcb != NULL){
		if(getTime() >= pcb->aTime){
			putOnCPU(pcb);
		}
	}

	if(!strcmp(alg, "FCFSP")){
		preemptive(q);
	} else if(!strcmp(alg, "FCFSN")){
		nonpreemptive(q);
	}
}

//Prints a usage string if the user enters a bad input
void printUsage(){
	printf("Usage: sched <inputfile> <scheduling algorithm> <scheduling parameters>\n");
	printf("	Support Schedulers:\n");
	printf("		1) FCFSP (preemptive) - Parameters: Quantum, Cost of half C.S.\n");
	printf("		2) FCFSN (non-preemptive) -  Parameters: Cost of C.S.\n");
}


int main(int argc, char *argv[]){
	int i = 0, j = 0, k = 0, pid = 1, quantum = 10, halfCS = 2, numBursts = 0, numLines = 1, arrivalTime = 0;
	char line[MAX_LINE_LENGTH], alg[MAX_ARG_LENGTH], fileName[MAX_ARG_LENGTH];
	int CPUbursts[MAX_BURSTS], IObursts[MAX_BURSTS-1], args[2*MAX_BURSTS + 1];
	memset(&args[0], 0, sizeof(args));
	memset(&CPUbursts[0], 0, sizeof(CPUbursts));
	memset(&IObursts[0], 0, sizeof(IObursts));

	FILE *infile;
	PCB *pcb;
	Queue *readyQueue, *waitQueue;
	readyQueue = malloc(sizeof(Queue)); //an actual queue to keep track of FCFS processes
	readyQueue->head = NULL;
	readyQueue->tail = NULL;
	waitQueue = malloc(sizeof(Queue));	//just a linked list to store waiting processes
	waitQueue->head = NULL;
	waitQueue->tail = NULL;

	//Initialize the CPU with the given queues
	initializeCPU(readyQueue, waitQueue);

	//If there is not enough arguments or the specified algorithm isn't supported
	if(argc <= 3){
		printUsage();
		return 0;
	} else if (strcmp(argv[2], "FCFSP") && strcmp(argv[2], "FCFSN")){
		printf("Unsupported algorithm: %s\n", argv[2]);
		printUsage();
		return 0;
	}

	//Parse the actual parameters for the CPU
	strcpy(fileName, argv[1]);
	strcpy(alg, argv[2]);
	if(!strcmp(alg, "FCFSP")){
		quantum = atoi(argv[3]);
		if(quantum <= 0){
			printf("Quantum must be a positive integer greater than 0.\n");
			printUsage();
			return 0;
		}
		halfCS = atoi(argv[4]);
		setCPUinfo(quantum, halfCS);

	} else if(!strcmp(alg, "FCFSN")){
		quantum = 0;
		halfCS = atoi(argv[3]);
		setCPUinfo(quantum, halfCS);
	}

	CPU * cpu = getCPU();
	infile = fopen(fileName,"r");
	if(infile == NULL){
		printf("The file named %s does not exist.\n", fileName);
		printUsage();
		return 0;
	}
	while(fgets(line, MAX_LINE_LENGTH, infile) != NULL){
		//Parse and decode line
		parseLine(args, line);
		i = 0, j = 0, k = 0;
		arrivalTime = args[i++];
		numBursts = args[i++];
		while(args[i] != NULL){
			if(i%2 == 0){
				CPUbursts[j] = args[i];
				j++;
			}
			else if(i%2 == 1){
				IObursts[k] = args[i];
				k++;
			}
			i++;
		}

		//Wait until arrivalTime to execute scheduling algorithm
		while(getTime() < arrivalTime){
			execAlg(alg, readyQueue);
			tick();
		}

		//Put the new process into the ready queue
		pcb = newPCB(pid++, arrivalTime, numBursts, CPUbursts, IObursts);
		printf("Process \"P%d\" has arrived at time %d.\n", pcb->pid, pcb->aTime);
		insert(readyQueue, pcb);
		cpu->newArrival = 1;

		//reset arrays
		memset(&args[0], 0, sizeof(args));
		memset(&CPUbursts[0], 0, sizeof(CPUbursts));
		memset(&IObursts[0], 0, sizeof(IObursts));
	}
	fclose(infile);
	cpu->endOfFile = 1;

	//Continue executing until no more processing is required
	while((readyQueue->head != NULL) || (waitQueue->head != NULL)|| (!idle())){
		execAlg(alg, readyQueue);
		tick();
	}

	//Overall Metrics
	int avgTAT=0,avgWT=0, avgRT=0, avgIO=0;
	float cpuUtil = 0.0, throughput = 0.0;

	Metrics * metrics = getMetrics();
	avgTAT = metrics->sumTAT/(pid-1);
	avgWT = metrics->sumWT/(pid-1);
	avgRT = metrics->sumRT/(pid-1);
	avgIO = metrics->sumIO/(pid-1);

	cpuUtil = (1.0 * metrics->burstTotal) / (getTime()-1);
	throughput = (1.0 * (pid-1)) / (getTime()-1);

	printf("All processes have terminated.\n");
	printf("OVERALL METRICS:\n");
	printf("    AvgTAT = %d, MaxTAT = %d\n", avgTAT, metrics->maxTAT);
	printf("    AvgWT = %d, MaxWT = %d\n", avgWT, metrics->maxWT);
	printf("    AvgRT = %d, MaxRT = %d\n", avgRT, metrics->maxWT);
	printf("    AvgIO = %d, MaxIO = %d\n", avgIO, metrics->maxIO);
	printf("    CPU Utilization = %f\n", cpuUtil);
	printf("    Throughput of %d processes in %d cycles or %f jobs per cycle\n", (pid-1), (getTime()-1), throughput);

	return 0;
}

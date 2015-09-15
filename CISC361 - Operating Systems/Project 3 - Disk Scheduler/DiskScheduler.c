/*
 * Scheduler.c
 *
 *  Created on: May 1, 2014
 *      Author: Anders Van Winkle
 *      Partner: Nick Nicolini
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "DiskScheduler.h"

//Globals

//Allocates and creates a new Read struct
Read* newRead(int aTime, int sector){
	int i;
	Read * read;
	read = malloc(sizeof(Read));
	read->sector = sector;
	read->aTime = aTime;
	read->pTime = 0;
	read->next = NULL;
	read->prev = NULL;
	return read;
}

//Insert at head
void insert(Queue *q, Read *item){
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

//Remove Read from Queue based on index
Read* indexRemove(Queue * q, int index){
	int i = 0;
    Read *currP, *prevP = NULL;
    for (currP = q->head; currP != NULL; prevP = currP, currP = currP->next) {
	    if (i == index) {
	    	if (prevP == NULL) {
	    		q->head = currP->next;
	    	} else {
	    		prevP->next = currP->next;
	    	}
	    	return currP;
	    }
	    i++;
    }
    return NULL; //NOT FOUND
}

//Remove from tail
Read* tailRemove(Queue *q){
	Read* obj = malloc(sizeof(Read));
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

//Returns the number of elements in the given Queue
int size(Queue * q){
	Read* temp = q->head;
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

//First Come First Serve scheduling algorithm
void fcfs(Disk *disk, Queue *arrivalQueue, Queue *finishQueue){
	if(disk->currRead == NULL){
		if(arrivalQueue->tail != NULL){
			Read *read = tailRemove(arrivalQueue);
			disk->currRead = read;
		} else{
			disk->time++;
		}
	} else {
		//The current read is finished
		if(disk->currRead->sector - disk->head == 0){
			disk->currRead->next = NULL;
			disk->currRead->prev = NULL;
			disk->currRead->pTime = disk->time;
			printf("Disk read processed at time %d for sector %d.\n", disk->time, disk->currRead->sector);
			insert(finishQueue, disk->currRead);
			disk->currRead = NULL;
		} else{
			int inc = (disk->currRead->sector - disk->head > 0) ? 1 : -1;
			disk->head += inc;
			disk->time += disk->sts;
		}
	}
}

//Shortest Seek Time Next scheduling algorithm
void sstn(Disk *disk, Queue *arrivalQueue, Queue *finishQueue){
	if(disk->currRead == NULL){
		if(arrivalQueue->head != NULL){
			Read *temp = arrivalQueue->head, *removed;
			int i = 0, minIndex = 0;

			//Initialize the min to the head of the queue's diff
			int diff = disk->head - temp->sector;
			if(diff < 0) diff *= -1;
			int min = diff;

			//Find minimum
			while(temp != NULL){
				diff = disk->head - temp->sector;
				if(diff < 0) diff *= -1;

				if(diff < min){
					minIndex = i;
					min = diff;
				}

				temp = temp->next;
				i++;
			}

			//Service the minimum seek time from the queue
			Read *read = indexRemove(arrivalQueue, minIndex);
			disk->currRead = read;
		}
		else {
			disk->time++;
		}
	} else {
		//The current read is finished
		if(disk->currRead->sector - disk->head == 0){
			disk->currRead->next = NULL;
			disk->currRead->prev = NULL;
			disk->currRead->pTime = disk->time;
			printf("Disk read processed at time %d for sector %d.\n", disk->time, disk->currRead->sector);
			insert(finishQueue, disk->currRead);
			disk->currRead = NULL;
		} else{
			int inc = (disk->currRead->sector - disk->head > 0) ? 1 : -1;
			disk->head += inc;
			disk->time += disk->sts;
		}
	}

}

//Scan scheduling algorithm
void scan(Disk *disk, Queue *arrivalQueue, Queue *finishQueue){
	int i = 0;
	Read *temp = arrivalQueue->head, *removed;
	while(temp != NULL){
		if(temp->sector == disk->head){
			removed = indexRemove(arrivalQueue, i);
			removed->next = NULL;
			removed->prev = NULL;
			removed->pTime = disk->time;
			printf("Disk read processed at time %d for sector %d.\n", disk->time, removed->sector);
			insert(finishQueue, removed);
		}
		temp = temp->next;
		i++;
	}

	//Increment time and move the head accordingly with the sector-to-sector seek time
	disk->time++;
	if(disk->time % disk->sts == 0){
		if(disk->head <= 0) disk->inc = 1;
		else if(disk->head > 9999) disk->inc = -1;
		disk->head += disk->inc;
	}
}

//Circular Scan scheduling algorithm
void cScan(Disk *disk, Queue *arrivalQueue, Queue *finishQueue){
	int i = 0;
	Read *temp = arrivalQueue->head, *removed;
	while(temp != NULL){
		//Service read request at the given sector
		if(temp->sector == disk->head){
			removed = indexRemove(arrivalQueue, i);
			removed->next = NULL;
			removed->prev = NULL;
			removed->pTime = disk->time;
			printf("Disk read processed at time %d for sector %d.\n", disk->time, removed->sector);
			insert(finishQueue, removed);
		}

		temp = temp->next;
		i++;
	}


	//If we reached the end, go to the beginning
	if(disk->head > 9999 && disk->eteFlag == 0){
		disk->eteFlag = 1;
		disk->eteStartTime = disk->time;
	}

	//Always increment time
	disk->time++;

	//Wait until disk finishes going end to end
	if(((disk->time - disk->eteStartTime) % disk->ete == 0) && disk->eteFlag){
		disk->head = 0;
		disk->eteFlag = 0;
	}
	//Move the head according to the sector-to-sector seek time
	else if(((disk->time - disk->eteStartTime) % disk->sts == 0) && !disk->eteFlag){
		disk->head += disk->inc;
	}
}

//Execute the given algorithm
void execAlg(char* alg, Disk* disk, Queue *arrivalQueue, Queue *finishQueue){
	if(!strcmp(alg, "FCFS"))		fcfs(disk, arrivalQueue, finishQueue);
	else if(!strcmp(alg, "SSTN"))	sstn(disk, arrivalQueue, finishQueue);
	else if(!strcmp(alg, "SCAN"))   scan(disk, arrivalQueue, finishQueue);
	else if(!strcmp(alg, "C-SCAN")) cScan(disk, arrivalQueue, finishQueue);
}

//Prints a usage string if the user enters a bad input
void printUsage(){
	printf("Usage: disksched <input file> <scheduling algorithm> <sector-to-sector seek time> <end-to-end seek time>\n");
	printf("	Supported Schedulers:\n");
	printf("		1) FCFS\n");
	printf("		2) SSTN\n");
	printf("		3) SCAN\n");
	printf("		4) C-SCAN\n");
}


int main(int argc, char *argv[]){
	char line[MAX_LINE_LENGTH], alg[MAX_ARG_LENGTH], fileName[MAX_ARG_LENGTH];
	int aTime, sector, args[2], lineCount = 0;

	//Initialize arrays
	memset(&args[0], 0, sizeof(args));

	//Initialize Disk
	Disk *disk;
	disk = malloc(sizeof(Disk));
	disk->head = 0;
	disk->time = 0;
	disk->inc = 1;
	disk->eteFlag = 0;
	disk->eteStartTime = 0;
	disk->currRead = NULL;

	//Declare Read pointer
	Read *read;

	//Declare file pointer
	FILE *infile;

	//Initialize queues
	Queue *arrivalQueue, *finishQueue;
	arrivalQueue = malloc(sizeof(Queue)); 		//an actual queue to keep track of FCFS reads
	arrivalQueue->head = NULL;
	arrivalQueue->tail = NULL;
	finishQueue = malloc(sizeof(Queue));	//just a linked list to store finished reads
	finishQueue->head = NULL;
	finishQueue->tail = NULL;

	//If there is not enough arguments or the specified algorithm isn't supported
	if(argc <= 4){
		printUsage();
		return 0;
	} else if (strcmp(argv[2], "FCFS") && strcmp(argv[2], "SSTN") && strcmp(argv[2], "SCAN") && strcmp(argv[2], "C-SCAN")){
		printf("Unsupported algorithm: %s\n", argv[2]);
		printUsage();
		return 0;
	}

	//Parse the actual parameters for the CPU
	strcpy(fileName, argv[1]);
	strcpy(alg, argv[2]);
	disk->sts = atoi(argv[3]);
	disk->ete = atoi(argv[4]);



	infile = fopen(fileName,"r");
	if(infile == NULL){
		printf("The file named %s does not exist.\n", fileName);
		printUsage();
		return 0;
	}
	while(fgets(line, MAX_LINE_LENGTH, infile) != NULL){
		//Parse and decode line
		parseLine(args, line);
		aTime = args[0];
		sector = args[1];

		//Wait until arrivalTime to execute scheduling algorithm
		while(disk->time < aTime){
			execAlg(alg, disk, arrivalQueue, finishQueue);
		}

		//Put the new process into the ready queue
		read = newRead(disk->time, sector);
		printf("Disk read arrived at time %d for sector %d.\n", disk->time, sector);
		insert(arrivalQueue, read);

		//reset arrays
		memset(&args[0], 0, sizeof(args));

		lineCount++;
	}
	fclose(infile);

	//Loop until all of the reads (every line of the file)
	while(size(finishQueue) < lineCount){
		execAlg(alg, disk, arrivalQueue, finishQueue);
	}


	//Overall Metrics
	int sumST = 0, maxST = 0;
	float diskUtil = 0, avgST = 0;

	Read * temp = finishQueue->head;
	while(temp != NULL){
		int seekTime = temp->pTime - temp->aTime;
		sumST += seekTime;
		if(seekTime > maxST) maxST = seekTime;

		temp = temp->next;
	}

	avgST = (1.0*sumST)/(1.0*lineCount);
	diskUtil = (1.0*lineCount)/(1.0*disk->time);

	printf("All processes have terminated.\n");
	printf("OVERALL METRICS:\n");
	printf("    AvgST = %f, MaxST = %d\n", avgST, maxST);
	printf("    Disk Utilization = %f\n", diskUtil);

	return 0;
}

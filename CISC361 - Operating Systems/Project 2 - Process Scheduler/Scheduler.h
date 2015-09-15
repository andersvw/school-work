/*
 * Scheduler.h
 *
 *  Created on: Apr 13, 2014
 *      Author: Anders Van Winkle
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#define MAX_LINE_LENGTH 1024
#define MAX_PROCS 1024
#define MAX_ARG_LENGTH 64
#define MAX_BURSTS 64

typedef struct PCB PCB;
typedef struct Queue Queue;

struct PCB{
	int pid;						//Process ID
	int aTime; 						//Arrival time
	int fTime; 						//Finish time
	int rTime; 						//Run time (first time on CPU)
	int numBursts;					//Number of CPU bursts
	int currNumBurstsRemaining;		//Number of CPU bursts remaining
	int CPUbursts[MAX_BURSTS];		//Array of all CPU burst lengths
	int IObursts[MAX_BURSTS-1];		//Array of all IO burst lengths
	int currCPUburst;				//Current CPU burst index
	int *currCPUburstRemaining;		//Current CPU burst length remaining (after decrementing while on CPU)
	int currIOburst;				//current IO burst index
	int *currIOburstRemaining;		//Current IO burst length remaining (after decrementing while in waitList)

	//flags
	int mustBeRestored; //Used to determine if a halfCS is needed to restore it's values

	//Pointers used to create doubly linked list structure
	PCB * next;
    PCB * prev;
};

struct Queue{
	PCB* head;
	PCB* tail;
};

void insert(Queue *q, PCB *item);
PCB* pidRemove(Queue *q, int pid);
void printContents(Queue * q);

#endif /* MAIN_H_ */

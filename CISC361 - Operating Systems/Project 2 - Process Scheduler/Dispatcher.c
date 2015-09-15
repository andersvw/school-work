/*
 * Dispatcher.c
 *
 *  Created on: Apr 14, 2014
 *      Author: Anders Van Winkle
 */

#include <stdlib.h>
#include <stdio.h>
#include "Scheduler.h"
#include "Dispatcher.h"

CPU * cpu = NULL;

void putOnCPU(PCB* pcb){
	if(idle()){
		if(!pcb->mustBeRestored){ //first time process runs
			pcb->rTime = cpu->time;
			pcb->mustBeRestored = 1;
		}  else{
			//Perform a halfCS to restore the process's registers
			halfCS();
			tick(); //needs 1 extra tick to be at the correct value
		}
		cpu->prev = cpu->pcb;
		cpu->pcb = pcb;
		printf("Process \"P%d\" is running at time %d for %d cycles.\n",cpu->pcb->pid, cpu->time, cpu->pcb->currCPUburstRemaining);
	}
}

void preempt(){
	int i, burstRemaining = cpu->pcb->currCPUburstRemaining;
	burstRemaining -= cpu->quantum;
	cpu->pcb->currCPUburstRemaining = burstRemaining;
	printf("Process \"P%d\" was preempted, and returned to Ready state at time %d.\n",cpu->pcb->pid, cpu->time);

	//Put the process back in the ready queue
	insert(cpu->readyQueue, cpu->pcb);

	//perform half CS to remove process from CPU
	halfCS();
}

void decWaitList(){
	PCB* temp = cpu->waitList->head, *removed;
	while(temp != NULL){
		int y = temp->currIOburstRemaining;
		y--;
		temp->currIOburstRemaining = y;
		if(y <= 0){
			//remove from the wait queue and put the process back in the ready queue
			removed = pidRemove(cpu->waitList, temp->pid);
			if(removed != NULL){
				printf("Process \"P%d\" has completed IO and returned to Ready state at time %d.\n", temp->pid, cpu->time);
				insert(cpu->readyQueue, removed);
			}
		}
		temp = temp->next;
	}
}

void tick(){
	//Decrement the current IOburst values for the processes waiting
	decWaitList();

	//Increment the cycle count
	cpu->time++;
}

//Returns the current cpu time
int getTime(){
	return cpu->time;
}

//Returns the actual CPU struct
CPU* getCPU(){
	return cpu;
}

//Returns the overall metrics struct
Metrics* getMetrics(){
	return cpu->metrics;
}

//Checks if the cpu is currently idle, i.e. does not have a process running
int idle(){
	if(cpu->pcb == NULL) return 1;
	else return 0;
}

//Allocates space and initializes the values for the CPU struct
void initializeCPU(Queue * rQueue, Queue * wQueue){
	cpu = malloc(sizeof(CPU));
	cpu->readyQueue = rQueue;
	cpu->waitList = wQueue;
	cpu->time = 0;
	cpu->quantum = 0;
	cpu->halfCS = 0;

	cpu->metrics = malloc(sizeof(Metrics));
	cpu->metrics->sumTAT = 0;
	cpu->metrics->maxTAT = 0;
	cpu->metrics->sumWT = 0;
	cpu->metrics->maxWT = 0;
	cpu->metrics->sumRT = 0;
	cpu->metrics->maxRT = 0;
	cpu->metrics->sumIO = 0;
	cpu->metrics->maxIO = 0;
	cpu->metrics->burstTotal = 0;

}

//Sends the parse command line arguments to the CPU
void setCPUinfo(int quantum, int halfCS){
	cpu->quantum = quantum;
	cpu->halfCS = halfCS;
}

//Perform a half context switch and take the currently running process off the CPU
void halfCS(){
	int i;
	for(i=0;i<cpu->halfCS-1;i++){
		tick();
	}
	cpu->pcb = NULL;
}

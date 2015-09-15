/*
 * Dispatcher.h
 *
 *  Created on: Apr 14, 2014
 *      Author: Anders Van Winkle
 */

#ifndef DISPATCHER_H_
#define DISPATCHER_H_

typedef struct CPU CPU;
typedef struct Metrics Metrics;

struct CPU{
	PCB * pcb;				//Keeps tack of the currently running process
	PCB * prev;				//Keeps track of the previously run process (used for determining CSs)****
	Queue * readyQueue;		//FCFS Queue of ready processes
	Queue * waitList;		//List of all processes waiting on IO
	Metrics * metrics;		//Struct used to keep track of overall metrics

	//Keep track of CPU cycles
	int time;

	//Simulation parameters from command line
	int quantum;
	int halfCS;

	//Simulator flags
	int endOfFile;
	int newArrival;
};

struct Metrics{
	int sumTAT, maxTAT, sumWT, maxWT, sumRT, maxRT, sumIO, maxIO, burstTotal;
};

void putOnCPU(PCB* pcb);
void preempt();
void tick();
void halfCS();
int idle();
void setCPUinfo(int quantum, int cs);
void initializeCPU(Queue * readyQueue, Queue * waitQueue);
int getTime();
CPU* getCPU();
Metrics* getMetrics();

#endif /* DISPATCHER_H_ */

/*
 * DiskScheduler.h
 *
 *  Created on: May 1, 2014
 *      Author: Anders Van Winkle
 */

#ifndef DISKSCHEDULER_H_
#define DISKSCHEDULER_H_

#define MAX_LINE_LENGTH 1024
#define MAX_PROCS 1024
#define MAX_ARG_LENGTH 64
#define MAX_BURSTS 64

typedef struct Read Read;
typedef struct Queue Queue;
typedef struct Disk Disk;

struct Read{
	int sector;			//Sector to be read
	int aTime; 			//Arrival time
	int pTime; 			//Processed time

	//Pointers used to create doubly linked list structure
	Read * next;
    Read * prev;
};

struct Queue{
	Read* head;
	Read* tail;
};

struct Disk{
	int sts;			//Sector-to-sector seek time
	int ete;			//End-to-end seek time
	int head;			//Index of the disk reader head
	int time;			//General clock
	int inc;			//Incremental value used for SCAN algorithm
	int eteFlag;		//Flag used for C-SCAN algorithm to determine if disk is jumping from end to end
	int eteStartTime;	//Time when the disk starts going from end to end
	Read * currRead;	//Currently executing disk read
};


#endif /* DISKSCHEDULER_H_ */

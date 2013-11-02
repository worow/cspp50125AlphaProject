#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include<sys/types.h>
#include<sys/stat.h>
#include<sys/sem.h>
#include<stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>

#include "binary_sems.h"
#include <stdarg.h>

#include "shmemMatchReportPub.h"
#include "messages.h"
const int tradePubSemIndex = 1;
const int bookPubIndex = 1;
const int reportingEngineSemIndex = 2;
const int reader = 0;
const int writer = 1;


class SharedMemory {
private:
	int* shm;
	int initSem(key keyVal, int index);
	vector<int> semIds;
	int numSemaphores; 
public:
	SharedMemory(int numSems);
	int* getShm() const;
	int getSemId(int index) const;
}

#endif
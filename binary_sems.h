#ifndef BINARY_SEMS_H
#define BINARY_SEMS_H

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

extern bool bsUseSemUndo;
extern bool bsRetryOnEintr;

int initSemAvailable(int semId, int semNum);

int initSemInUse(int semId, int semNum);

int reserveSem(int semId, int semNum);

int releaseSem(int semId, int semNum);


#endif
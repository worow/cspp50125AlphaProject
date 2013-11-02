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

void * init_match_shmem(){

	int shmid;
  	int *shm;
  	key_t key = SHM_KEY;
  	int flag = SHM_R | SHM_W;

  	shmid = shmget(key, sizeof(struct TradeMessage), flag);
  	
  	if (shmid < 0) {
    	shmid = shmget(key, 1000, flag | IPC_CREAT);
    	if (shmid < 0) {
      	perror("shmget | IPC_CREAT");
    	}
  	}

  	shm = (int *)shmat(shmid, /*addr*/NULL, /*flag*/0); 
  	
  	if (shm == NULL) {
    	perror("shmat");
    	exit(1);
  	}
  	
  	return shm;
}


int init_sem_A(){
	
	int semId;
	semId =  semget(SEM_KEY_A, 2, IPC_CREAT | OBJ_PERMS);
	
	if(initSemAvailable(semId, MATCH_WRITE_SEM)==-1)
		exit(-1);
	if(initSemInUse(semId, PUB_READ_SEM)==-1)
		exit(-2);
	
	return semId;
} 

int init_sem_B(){

	int semId;
	semId =  semget(SEM_KEY_B, 2, IPC_CREAT | OBJ_PERMS);
	
	if(initSemAvailable(semId, MATCH_WRITE_SEM)==-1)
		exit(-1);
	if(initSemInUse(semId, PUB_READ_SEM)==-1) /*Redefine PUB_READ_SEM*/
		exit(-2);
	
	return semId;

}

int init_sem_C(){
	
	
	return semget(SEM_KEY_C, 2, IPC_CREAT | IPC_EXCL | OBJ_PERMS);

	
}
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/sem.h>
#include<stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <string.h>     

#include "messages.h"
#include "shmemMatchReportPub.h"
#include "binary_sems.h"

int main(int argc, char** argv){

/* Creating/attaching to shared memory */

    struct TradeMessage *trdmsg;
    trdmsg = (struct TradeMessage *) init_match_shmem();
    
/* Creating attaching to semaphore set */

	int semId;
	semId = init_sem_A();

/*Creating fake trades*/

	int i=1;
	
	char sym[16] = "MSFT";
	char p[10] = "24.5";
	
	struct TradeMessage *trade;
	trade = (struct TradeMessage*) malloc(sizeof(TradeMessage));
	
	memcpy(trade->symbol, sym, 16);
	memcpy(trade->price,p,10);
	trade->quantity = (unsigned long) i;
	
	while(1){
	    reserveSem(semId,MATCH_WRITE_SEM);
		memcpy(trdmsg,trade,sizeof(TradeMessage));
		releaseSem(semId, PUB_READ_SEM);
	    printf("Trade Published %s %s %ld\n", trdmsg->symbol, trdmsg->price, trdmsg->quantity);
	    trade->quantity = (unsigned long) i++;
	}


	return 1;


}
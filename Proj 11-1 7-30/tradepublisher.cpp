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

#define BUFSIZE sizeof(TradeMessage)+16

const char* IP_ADDR = "239.192.03.03";
const char* PORT = "1234";


int main(int argc, char** argv){

/* Setting up DGRAM multicast socket */
    int sockfd, portno;
    struct sockaddr_in sock_addr/*, sender_addr*/;
	/*struct ip_mreq mc_req;*/
	char trademsg[BUFSIZE];
	unsigned char mcttl = 1;

	sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sockfd == -1)
		perror("socket");
	
	if(setsockopt(sockfd, IPPROTO_IP,IP_MULTICAST_TTL, (void*) &mcttl,sizeof(mcttl)) == -1)
		perror("socket");
	
	memset((char *) &sock_addr,0,sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;
	portno = atoi(PORT);
	sock_addr.sin_port = htons(portno);
	sock_addr.sin_addr.s_addr = inet_addr(IP_ADDR);

/* Creating/attaching to shared memory */

    struct TradeMessage *trdmsg;
    trdmsg = (struct TradeMessage *) init_match_shmem();
    
/* Creating attaching to semaphore set */

	int semId;
	semId = init_sem_A();
	printf("ready to read\n");
	while(1){
		
		memset(trademsg, 0, BUFSIZE);
		reserveSem(semId,PUB_READ_SEM);
		memcpy(&trademsg,&trdmsg,BUFSIZE);
		printf("Trade: %s %s %ld\n",trdmsg->symbol, trdmsg->price, trdmsg->quantity);
		releaseSem(semId, MATCH_WRITE_SEM);
		if(sendto(sockfd,(void*) trademsg, BUFSIZE,0,(struct sockaddr*) &sock_addr, sizeof(sock_addr))!= BUFSIZE){
			perror("Sendto");
			exit(EXIT_FAILURE);
		
		}
	}
	
	return 1;

}


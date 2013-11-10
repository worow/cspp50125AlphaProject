#include<stdio.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<time.h>
#include<arpa/inet.h>
#include "message.h"

#define BUFSIZE sizeof(struct TradeMessage) + 16;

const char* IP_ADDR = "239.192.03.03";
const char* PORT = "1234";

int main(int argc, char** argv){
	
	int sockfd, optval, portno, frmlen;
	struct sockaddr_in sock_addr, sender_addr;
	struct ip_mreq mc_req;
	struct TradeMessage *tm;
    tm = (struct TradeMessage*) malloc(sizeof(struct TradeMessag));
    

	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sockfd == -1)
		perror("socket");
	
	optval = 1;
	if(setsockopt(sockfd, SOL_SOCKET,SO_REUSEADDR, &optval,sizeof(optval)) == -1)
		perror("socket");
	
	
	memset((char *) &sock_addr,0,sizeof(sock_addr));
	sock_addr.sin_family = htonl(AF_INET);
	
	portno = PORT;
	sock_addr.sin_port = htons(portno);
	
	sock_addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(sockfd, (struct sockaddr*) &sock_addr, sizeof(struct sockaddr_in)) == -1){
		perror("bind");
		}


	memset(&mc_req,0,sizeof(struct ip_mreq));
	mc_req.imr_multiaddr.s_addr = inet_addr(IP_ADDR);
	mc_req.imr_interface.s_addr = htonl(INADDR_ANY);

	if(setsockopt(sockfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,(void*) &mc_req,sizeof(struct ip_mreq))<0){
		perror("setsockopt");
		exit(EXIT_FAILURE);
		}
	
	frmlen = sizeof(sender_addr);
	while(1){

		memset(timemsg, 0, BUFSIZE);
		memset(&sender_addr,0,sizeof(struct sockaddr_in));
		if(recvfrom(sockfd, tm,BUFSIZE, 0,(struct sockaddr*) &sender_addr,(socklen_t*) &frmlen) < 0){
			perror("recvfrom");
			exit(EXIT_FAILURE);
		}
		
		printf("Trade: %s %s %ld\n", tm->symbol,tm->price,tm->quantity);
		
		
		

	}

	return 1;

}

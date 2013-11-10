#include<sys/types.h>
#include<sys/stat.h>
#include<sys/sem.h>
#include "messages.h"
#include<stdlib.h>


#define SHM_KEY 0x1234

#define SEM_KEY_A 0x5678
#define SEM_KEY_B 0x9678
#define SEM_KEY_C 0x7678

#define OBJ_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)

#define MATCH_WRITE_SEM 0
#define PUB_READ_SEM 1
#define PUB_SEM 2

#ifndef BUF_SIZE
#define BUF_SIZE sizeof(struct TradeMessage)
#endif+

struct shmseg {
    int cnt;
    char buf[BUF_SIZE];
};

/**** Functions to Use IPC ******/

int init_sem_A();        /*Called by the matching engine to initialize semaphore set, matching engine must start before trade recorder and trade publisher*/
void * init_match_shmem();   /*Initializes shared memory for the matching engine, return value should be cast to pointer to TradeMessage*/

int init_sem_B();         /*Initialize semaphore set for trade publisher*/
void * init_pub_shmem();    /*Attach trade publisher to shared memory, return value should be cast to pointer to TradeMessage*/

int init_sem_C();         /*Initialize semaphore set for trade reporter*/
void * init_rep_shmem();    /*attach trade reporter to shared memory, return value should be cast to pointer to TradeMessage*/

int push_trade(struct TradeMessage *tm, struct TradeMessage trade); /*Function to put new trades into shared memory, tm is a pointer to the shared memory return by init_match_shem
                                                                       trade is the trade to be reported */

int pub_get_trade(struct TradeMessage *tm);     
                                              /*by analogy to above*/
int rep_get_trade(struct TradeMessage *tm);


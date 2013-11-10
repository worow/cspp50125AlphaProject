#include<sys/types.h>
#include<sys/stat.h>
#include<sys/sem.h>
#include<stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>     
#include <sqlite3.h>


#include "messages.h"
#include "shmemMatchReportPub.h"
#include "binary_sems.h"

#define BUFSIZE sizeof(TradeMessage)+16

/*Require function for DB operation*/

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}



int main(int argc, char** argv){

	/* Attaching to shared Memory*/

	struct TradeMessage *trdmsg;
    trdmsg = (struct TradeMessage *) init_match_shmem();

	/* Initializing Semaphore Set */

	int semId;
	semId = init_sem_B();
	printf("ready to read\n");
	
	/* Creating or connecting to database */
	
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open("test.db", &db);

	if( rc ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}else{
		fprintf(stderr, "Opened database successfully\n");
	}
	
	/* Creating Table */
	char* sql;
	sql = "CREATE TABLE TRADES("  \
			"ID INT PRIMARY KEY     NOT NULL," \
			"SYMBOL           TEXT    NOT NULL," \
			"PRICE            TEXT     NOT NULL," \
			"QUANTITY        UNSIGNED BIG INT NOT NULL);";

   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Table created successfully\n");
   }

	char record[128];
	int id = 1;
	while(1){
		
		
		memset(record,0,128);
		reserveSem(semId,PUB_READ_SEM);
		
		printf("Trade: %s %s %ld\n",trdmsg->symbol, trdmsg->price, trdmsg->quantity);
		sprintf(record, "INSERT INTO TRADES VALUES (%d, '%s', '%s', %ld);",id++, trdmsg->symbol, trdmsg->price, trdmsg->quantity);
		printf("%s\n", record);
		rc = sqlite3_exec(db, record, callback, 0, &zErrMsg);
   		
   		if( rc != SQLITE_OK ){
      		fprintf(stderr, "SQL error: %s\n", zErrMsg);
      		sqlite3_free(zErrMsg);
   		}else{
      		fprintf(stdout, "Records created successfully\n");
  		}
				

		releaseSem(semId, MATCH_WRITE_SEM);
		
	}
	
	/*Need pleasant way to dispose of semaphore, detach from memory and close database*/
	
	return 1;
	
	


}
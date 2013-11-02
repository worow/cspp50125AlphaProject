CC=g++
FLAGS= -Wall -ansi

all: tradePub matchingEngine msgBus reporter

tradePub: tradepublisher.o shmemMatchReportPub.o binary_sems.o
	$(CC) $(FLAGS) -o tradePub tradepublisher.o shmemMatchReportPub.o binary_sems.o

matchingEngine: match.o shmemMatchReportPub.o binary_sems.o
	$(CC) $(FLAGS) -o matchingEngine match.o shmemMatchReportPub.o binary_sems.o
	
reporter: reporter.o shmemMatchReportPub.o binary_sems.o
	$(CC) $(FLAGS) -o reporter reporter.o shmemMatchReportPub.o binary_sems.o -l sqlite3
	
msgBus: multicastbus.o
	$(CC) $(FLAGS) -o msgBus multicastbus.o

match.o: match.cpp shmemMatchReportPub.h binary_sems.h
	$(CC) $(FLAGS) -c match.cpp -I.

tradepublisher.o: tradepublisher.cpp shmemMatchReportPub.h
	$(CC) $(FLAGS) -c tradepublisher.cpp -I.
	
shmemMatchReportPub.o: shmemMatchReportPub.cpp shmemMatchReportPub.h
	$(CC) $(FLAGS) -c shmemMatchReportPub.cpp -I.
	
binary_sems.o: binary_sems.cpp binary_sems.h semun.h
	$(CC) $(FLAGS) -c binary_sems.cpp -I.

multicastbus.o: multicastbus.cpp messages.h
	$(CC) $(FLAGS) -c multicastbus.cpp -I.

reporter.o: reporter.cpp shmemMatchReportPub.h
	$(CC) $(FLAGS) -c reporter.cpp -I. 

	

clean:
	\rm -f *.o main match msgBus matchingEngine tradePub *~
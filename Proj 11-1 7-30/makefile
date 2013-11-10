CC=g++
FLAGS= -Wall -ansi

main: tradepublisher.o shmemMatchReportPub.o binary_sems.o
	$(CC) $(FLAGS) -o main tradepublisher.o shmemMatchReportPub.o binary_sems.o


tradepublisher.o: tradepublisher.cpp shmemMatchReportPub.h
	$(CC) $(FLAGS) -c tradepublisher.cpp -I.
	
shmemMatchReportPub.o: shmemMatchReportPub.cpp shmemMatchReportPub.h
	$(CC) $(FLAGS) -c shmemMatchReportPub.cpp -I.
	
binary_sems.o: binary_sems.cpp binary_sems.h semun.h
	$(CC) $(FLAGS) -c binary_sems.cpp -I.


	

clean:
	\rm -f *.o main *~
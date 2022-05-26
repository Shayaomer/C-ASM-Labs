all: task1

task1: task1.o task1.c LineParser.o
	gcc -g -Wall -o task1 task1.o LineParser.o

task1.o: task1.c
	gcc -g -Wall -c -o task1.o task1.c 

lineParser.o: lineParser.c
	gcc -g -Wall -c -o task1.o task1.c

.PHONY = clean

clean: 
	rm -f *.o task1

all: lab3_mergesort.exe

lab3_mergesort.exe: lab3_mergesort.o
	gcc -g -Wall -o lab3_mergesort.exe lab3_mergesort.o

lab3_mergesort.o: lab3_mergesort.c
	gcc -g -Wall -c -o lab3_mergesort.o lab3_mergesort.c

.PHONY = clean

clean: 
	rm -f *.o *.exe

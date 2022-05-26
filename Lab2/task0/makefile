all: count-words.exe

count-words.exe: count-words.o
	gcc -g -Wall -o count-words.exe count-words.o

count-words.o: count-words.c
	gcc -g -Wall -c -o count-words.o count-words.c

.PHONY = clean

clean:
	rm -f *.o *.exe



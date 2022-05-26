all: myshell

myshell: myshell.o myshell.c LineParser.o
	gcc -g -Wall -o myshell myshell.o LineParser.o

myshell.o: myshell.c
	gcc -g -Wall -c -o myshell.o myshell.c 

lineParser.o: lineParser.c
	gcc -g -Wall -c -o myshell.o myshell.c

.PHONY = clean

clean: 
	rm -f *.o myshell

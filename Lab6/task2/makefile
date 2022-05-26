all: shell

shell: shell.o LineParser.o
	gcc  -g -Wall -o shell shell.o LineParser.o 

shell.o: shell.c
	gcc -g -Wall -c -o shell.o shell.c

LineParser.o: LineParser.c
	gcc  -g -Wall -c -o LineParser.o LineParser.c

clean:
	rm -f *.o shell

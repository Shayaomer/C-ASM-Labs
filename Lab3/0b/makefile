all: hexaPrint.exe

hexaPrint.exe: hexaPrint.o
	gcc -g -Wall -o hexaPrint.exe hexaPrint.o

hexaPrint.o: hexaPrint.c
	gcc -g -c -Wall -o hexaPrint.o hexaPrint.c

.PHONY = clean

clean:
	rm -f *.o *.exe

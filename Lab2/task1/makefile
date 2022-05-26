all: addresses[1].exe

addresses[1].exe: addresses[1].o
	gcc -g -m32 -Wall -o addresses[1].exe addresses[1].o

addresses[1].o: addresses[1].c
	gcc -g -m32 -Wall -c -o addresses[1].o addresses[1].c

.PHONY = clean

clean:
	rm -f *.o *.exe

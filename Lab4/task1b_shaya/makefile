all: program

program: start util count
	ld -m elf_i386 start.o count.o util.o -o count

start: start.s
	nasm -f elf32 start.s -o start.o

util: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o

count: count.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector count.c -o count.o

.PHONY = clean

clean: 
	rm -f *.o count

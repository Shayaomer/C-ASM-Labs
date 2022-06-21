all: task2

task2: skeleton.o
	ld -m elf_i386 skeleton.o -o task2

skeleton.o: skeleton.s
	nasm -f elf32 skeleton.s -o skeleton.o

.PHONY = clean

clean:
	rm -f *.o task2

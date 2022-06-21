all: skeleton

skeleton: skeleton.o
	ld -m elf_i386 -o skele skeleton.o

skeleton.o: skeleton.s
	nasm -f elf32 skeleton.s -o skeleton.o

.PHONY: skeleton
clean: 
	rm -f *.o skele
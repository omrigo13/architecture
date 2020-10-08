section	.rodata
decimal:
	DB	"%d", 10, 0	; convert to decimal Format string

section	.data
	answer:    DD 0

section .text
	global assFunc
	extern printf
	extern c_checkValidity

assFunc:
	push	ebp
	mov	ebp, esp       	   ; set up ebp and esp
	sub esp, 4             ; allocate space for local var
    mov ecx, [ebp+12]      ; Get second argument (y)
	mov ebx, [ebp+8]       ; Get first argument (x)
	mov [ebp-4], ebx       ; move x to the stack
	pushad                 ; backup registers
	pushfd                 ; backup eflags
	push ecx               ; Call c_checkValidity(x, y)
	push ebx
	call c_checkValidity
	mov [answer], eax      ; move answer to eax
	add esp, 8             ; Clean up stack after call
	popfd                  ; Restore eflags
	popad                  ; Restore registers
	cmp dword [answer], 1  ; if x >= y jump to bigger
	je bigger
	add ebx, ecx
	mov eax, ebx       	   ; eax <- z = x + y
	pushad                 ; backup registers
    pushfd                 ; backup eflags
    push	eax	       	   ; Call printf with 2 arguments: answer (z)
    push	decimal	       ; and pointer to format number
    call	printf
    add 	esp, 8	       ; Clean up stack after call
    popfd                  ; restore eflags
    popad		           ; Restore registers
    jmp end_func
	bigger:
		sub ebx, ecx
		mov eax, ebx		; eax <- z = x - y
    	pushad              ; backup registers
    	pushfd              ; backup eflags
    	push	eax	        ; Call printf with 2 arguments: answer (z)
    	push	decimal	    ; and pointer to format number
    	call	printf
    	add 	esp, 8	    ; Clean up stack after call
    	popfd               ; restore eflags
    	popad		        ; Restore registers
end_func:
	mov	esp, ebp	        ; Function exit code
    pop	ebp
    ret
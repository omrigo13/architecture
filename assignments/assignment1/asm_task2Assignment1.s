section	.rodata			; we define (global) read-only variables in .rodata section
	format_string: db "%s", 10, 0	; format string

section .bss			; we define (global) uninitialized variables in .bss section
	an: resb 12		; enough to store integer in [-2,147,483,648 (-2^31) : 2,147,483,647 (2^31-1)]

section .text
	global convertor
	extern printf

convertor:
	push ebp
	mov ebp, esp	
	pushad			

	mov ecx, dword [ebp+8]	; get function argument (pointer to string)
	
	sub esp, 4              ; space in stack for pointer to string
	mov [ebp-4], esp        ; pointer to the first char in the string
	mov eax, 0
	asciiToDec:
		mov al, [ecx]       ; gets the first byte in ecx
		cmp al, 10          ; check if input is \n
		je my_code
		inc ecx             ; ecx++ for next byte
		sub al, 48          ; convert al to decimal
		push eax            ; push the result of decimal back to the stack
	jne asciiToDec

	my_code:
		mov ecx, 0          ; sums the decimal numbers to one number
		mov ebx, 1          ; multiplier

	sum:
		pop eax             ; get the digit
		mul ebx             ; multiply by 10^ebx (1,10,100...)
		add ecx, eax        ; add the total number until now to the sum
		mov eax, 10         ; multiplicand
		mul ebx             ; multiplier, ebx = 10*ebx
		mov ebx, eax
		mov edx, esp
		sub edx, [ebp-4]    ; checks if there are more digits to sum or not
	jnz sum

	mov eax, ecx            ; copy the sum to eax
	mov ebx, 16             ; divisor to convert sum from decimal to hexa
	mov [ebp-4], esp        ; pointer to the first char in the string

	division:
		div ebx             ; div eax by 16
		mov ecx, edx        ; copy reminder to ecx
		cmp ecx, 9
		jle isDigit
		add edx, 7          ; add 7 to convert digit between 10-15 to A-F ascii (65-70)
		isDigit:
			add edx, 48     ; add 48 to convert digit between 0-9 to ascii (48-57)
			push edx        ; insert the char back to the stack
			mov edx, 0      ; correctness division
			cmp eax, 0      ; if eax = 0 division finished
	jnz division

	mov edx, an             ; pointer to save

	end_division:
		pop ecx             ; get next char
		mov [edx], ecx      ; insert the next result char
		inc edx
		mov ebx, [ebp-4]    ; check if there are more digits or not
		sub ebx, esp
	jnz end_division

func_end:
	push an			; call printf with 2 arguments -  
	push format_string	; pointer to str and pointer to format string
	call printf
	add esp, 12		; clean up stack after call

	popad			
	mov esp, ebp	
	pop ebp
	ret
section .bss
    stack: resd 1 ; address of the operand stack
    in: resb 81   ; the input buffer from the user - 81, 80 for max input and another one to null terminator 
    tempNode: resb 1
section .rodata
    format_number: db "%d", 10, 0	; convert to number Format string
    format_string: db "%s", 10, 0	; convert to string
    format_hexa: db "%X", 10,0      ; convert to hexa format
    newLine: db 10, 0               ; '\n'
    calcMsg: db "calc: ", 0         ; prints "calc: for every input"
    stack_overflow_error: db 'Error: Operand Stack Overflow',10,0           ; no free space on the operand stack
    stack_empty: db 'Error: Insufficient Number of Arguments on Stack',10,0 ; if an operation attempts to pop an empty stack
    
    DATA: equ 0 ; Offset of the value byte from the beginning of a node
    NEXT: equ 1 ; Offset of the next-node field (4 bytes) from the beginning of a node

section .data
    debug: db 0             ; 1 if debug mode is on and 0 otherwise
    sumDigits: db 0         ; sums the number of digits for number of hexadecimal digits function
    stackSize: db 5         ; Operand stack size between 2-FF and 5 by default
    countItems: db 0        ; current number of items in the stack
    countZeros: db 0        ; zeros counter of a number
    countMoves: dd 0 ; counter for the number of operations making on the stack 
    node1: dd 0             ; pointer to a node to free from the stack
    node2: dd 0             ; second pointer to a node to free from the stack

section .text                    	
    align 16
    global main
    extern printf
    extern fprintf 
    extern fflush
    extern malloc 
    extern calloc 
    extern free 
    extern getchar 
    extern fgets
    extern stdout
    extern stderr

; bc we have return value in eax we want to backup all the registers except eax
%macro pushRegs 0
    push edx
    push ecx
    push ebx
    push esi
    push ebp
%endmacro

%macro popRegs 0
    pop ebp
    pop esi
    pop ebx
    pop ecx
    pop edx
%endmacro

; printing value of end operation if debug mode is on
%macro endOperationDebugMode 1
    cmp byte [debug], 1
    jne %1
    pushad
    mov ebx, [stack]
    mov edx, 0
    mov dl, [countItems]
    dec dl
    push dword [stderr]
    push dword [ebx + edx * 4]
    call popAndPrintLoop
    add esp, 8
    push newLine
    call printf
    add esp, 4
    push dword [stderr]
    call fflush
    add esp, 4
    popad
%endmacro

main:
    mov ebp, esp                ; set up ebp and esp
    mov ecx, [esp+4]            ; ecx = argc
    mov ebx, [esp+8]            ; ebx = pointer to the args array, argv
    add ebx, 4                  ; we don't need argv[0], its file path
    dec ecx                     ; updates the argc we have to check, argc not including file path arg
    jz init                     ; start calcMain if there are not arguments, no debug mode and stack size = 5

    parseArgs:
        mov edx, [ebx]          ; edx = argv[i], loop over the args array starting on argv[1]
        mov edx, [edx]          ; edx = argv[i][0]
        
        parseDebugMode:
            and edx, 0x00FFFFFF ; Now the first byte (last char of a string) of edx is zeros
            cmp edx, 0x0000642D ; "-d\0\0" (The first \0 is due to the previous line)
            jne parseStackSize
            
            mov byte [debug], 1 ; debug = 1 , if we are here we got -d flag on args array
            jmp endParseArgs

        parseStackSize:
            pushad
            push word dx        
            call HexaToByte     ; call convert function to transfer hexa in ascii to byte
            add esp, 2          ; update stack to point after we converted the stack new size
            mov [stackSize], al ; updates stackSize to the hexa we got as parameter
            popad
            
        endParseArgs:
            add ebx, 4          ; forward to the next parameter, relevant for option of new stack size and -d flag
            loop parseArgs, ecx ; decrement ecx and if ecx != 0, jump tp parseArgs to parse next arguments


init:
    mov eax, 4 
    push eax
    mov eax, [stackSize]        ; allocate on eax space of StackSize
    inc eax                     ; allocate an extra space to be used in pushByteToHexa.
    push eax   
    call calloc        
    add esp, 8                  ; cleaning the stack from locals
    mov dword[stack], eax       ; eax is the pointer to the start of the stack
    

;~~~~~~~myCalc~~~~~~~;

calcMain:
    push calcMsg
    call printf
    add esp, 4
    push dword [stdout]                 ; prints "calc: " on every line
    call fflush
    add esp, 4
    mov eax, 3                          ; system call of SYS_READ, reads the input to the in buffer, eax saves the input from the user
    mov ebx, 0
    mov ecx, in
    mov edx, 81
    int 0x80 

    pushad
    mov esi, in                         ; put the user input from the buffer in esi register
    NullLoop:
        cmp byte [esi], 10              ; end of input when \n
        je endNullLoop
        inc esi
        jmp NullLoop
    endNullLoop:
    mov byte [esi], 00                  ; add null terminator at the end of the user input 
    popad 

    dec eax                             ; input of the user without the \n

    cmp byte [in], 'q'
    jne call_addition
    jmp endCalcMain                     ; perform quit operation
      
    call_addition: cmp byte [in], '+'
        jne call_popAndPrint
        call addition                   ; perform add operation
        jmp calcMain
        
    call_popAndPrint: cmp byte [in], 'p'
        jne call_duplicate
        call popAndPrint                ; perform pop and print operation
        jmp calcMain

    call_duplicate: cmp byte [in], 'd'
        jne call_bitWiseAnd
        call duplicate                  ; perform duplicate operation
        jmp calcMain

    call_bitWiseAnd: cmp byte [in], '&'
        jne call_bitWiseOr
        call bitwiseAnd                 ; perform bitwise and operation
        jmp calcMain

    call_bitWiseOr: cmp byte [in], '|'
        jne call_numDigits
        call bitwiseOr                  ; perform bitwise or operation
        jmp calcMain

    call_numDigits: cmp byte [in], 'n'
        jne numberInput
        call numDigits                  ; perform number of hexa digits operation
        jmp calcMain

    
    numberInput:                        ; we got number input and we need to add it to the operand stack
        push eax
        call pushByteToHexa             ; parse the number input from hexa in ascii to real hexa in our stack
        add esp, 4                      ; updates the stack pointer after we add the number to the stack
        jmp calcMain

endCalcMain:                            ; ends the calc after q operation, free the stack and print number of operations
    pushad
    mov ebx, stack
    freeLoop:                           ; free the stack and the nodes in it
        cmp byte [countItems], 0
        je endFreeLoop
        call popNode
        pushad
        push eax
        call freeLinkedList
        add esp, 4
        popad
        jmp freeLoop
    endFreeLoop:
    push dword [stack]
    call free
    add esp, 4
    popad
    mov eax, [countMoves]        ; print number of operations we did during the using of the calculator
    pushad
    push eax
    push format_number
    call printf
    add esp, 8
    popad
    mov ebx, 0
    mov eax, 1
    int 0x80                            ; system call of exit(0) to end the program

;~~~~~~~unsigned addition~~~~~~~;

addition:
    mov ebp, esp                                ; set up ebp and esp
    inc dword [countMoves]               ; increment by 1 the operations counter

    push ebp                                    ; backup ebp pointer
    call popTwoNodesFromStack                   ; pop two elements from the top of the stack for addition between them
    pop ebp                                     ; restore ebp pointer
    cmp eax, 0                                  ; eax saves the second element and if its 0 so there are no 2 values to add between them so we finish
    je additionEnd

    mov ecx, eax                                ; ecx saves the second element
    mov [node1], ecx                            ; updates the second node we have to free from stack, ecx = Y
    mov [node2], ebx                            ; updates the first node we have to free from stack, ebx = X
    pushRegs
    call createNodeOnOperandStack               ; creates a new node that will hold the sum of the two nodes we free
    popRegs
    clc                                         ; Clear Carry Flag
    pushfd
    mov edx, 0                                  ; reset edx
    additionLoop:
        popfd
        mov byte dl,[ebx + DATA]                ; dl = X, first node of X
        mov byte [eax + DATA], dl               ; eax = X, first node of X
        mov byte dl,[ecx + DATA]                ; dl = Y, first node of Y
        adc byte [eax + DATA], dl               ; eax = X+Y
        pushfd
        
        mov ebx, [ebx + NEXT]                   ; updates ebx to the next node in X
        mov ecx, [ecx + NEXT]                   ; updates ecx to the next node in Y

        mov edx, ecx                            ; copy the next node of Y to edx
        cmp ebx, 0                              ; ebx saves the next node of the first element and if its 0 so there are no more nodes to add with carry
        je additionRest

        mov edx, ebx                            ; copy the next node of X to edx
        cmp ecx, 0
        je additionRest                         ; ebx saves the next node of the first element and if its 0 so there are no more nodes to add with carry                

        mov edx, eax                            ; edx is the value of a byte that we want to save in a new node
        pushRegs
        call createNode
        popRegs
        mov [edx + NEXT], eax                   ; after we create the new node we add it to the next place
        jmp additionLoop

        additionRest:                           ; edx has the other var
            cmp edx, 0                          ; we finished with addition of X and Y
            je lastCarry
        
            lastAdditionLoop:                   ; loop until edx is empty
                mov ecx, eax
                pushRegs
                call createNode
                popRegs
                mov [ecx + NEXT], eax           ; after we create the new node we add it to the next place

                popfd
                mov cl, [edx + DATA]
                adc [eax + DATA], cl
                mov edx, [edx + NEXT]           ; after we create the new node we add it to the next place
                pushfd
                jmp additionRest

            lastCarry:
                popfd
                jnc additionEndFree             ; checking if we have carry to add
                mov edx, eax
                pushRegs
                call createNode
                popRegs
                mov [edx + NEXT], eax
                inc byte [eax + DATA]
                
        additionEndFree:
            pushad
            push dword [node1]                  ; free the second node from the stack, node1 = Y
            call freeLinkedList
            add esp, 4

            push dword [node2]                  ; free the first node from the stack, node2 = X
            call freeLinkedList
            add esp, 4
            popad
        cmp byte [sumDigits], 1
        je additionEnd
        endOperationDebugMode additionEnd       ; print for debug mode
    additionEnd:
        mov esp, ebp
        ret

;~~~~~~~pop-and-print~~~~~~~;

popAndPrint:
    mov ebp, esp                                ; set up ebp and esp
    inc dword [countMoves]               ; increment by 1 the operations counter
    
    call popNode
    cmp eax, 0                                  ; pop node from the top of the operand stack
    je popAndPrintEnd
    mov ebx, eax                                ; ebx = the popped node from the top of the stack

    pushad                                      ; these lines prints the top of the stack that is saved on ebx and updates the stack pointer to a new top 
    push dword [stdout]
    push ebx
    call popAndPrintLoop
    add esp, 8
    push newLine
    call printf
    add esp, 4
    push dword [stdout]
    call fflush
    add esp, 4
    popad

    pushad
    push ebx
    call freeLinkedList
    add esp, 4
    popad
    
    popAndPrintEnd:
        mov esp, ebp
        ret

popAndPrintLoop:
    mov ebp, esp                                ; set up ebp and esp
    push dword 0                                ; set up a value that will be the top of the stack that we want to print
    
    mov ebx, [ebp+4]                            ; updates the new top of the stack in ebx
    mov cx, [ebx + DATA]                        ; cx = the value of the first node of the previous element
    mov edx, [ebx + NEXT]                       ; edx = the value of the next node of the previous element
    cmp edx, 0
    je lastpopAndPrintLoop                      ; if we got no previous element it means we are trying to pop the last element from the stack

    pushad
    push dword [ebp+8]
    push edx
    call popAndPrintLoop
    add esp, 8
    popad

    printNode:                                  ; converts the bytes to hexa and then print it
        pushRegs
        push cx
        call byteToHexa
        add esp, 2
        popRegs
        
        and eax, 0x0000FFFF
        mov [ebp-4], eax
        mov edx, ebp
        sub edx, 4
        pushad
        push edx
        call printf
        add esp, 4
        push dword [ebp+8]
        call fflush
        add esp, 4
        popad
    mov esp, ebp
    ret

    lastpopAndPrintLoop:                        ; prints the last node
        pushRegs
        push cx
        call byteToHexa
        add esp, 2
        popRegs

        and eax, 0x0000FFFF
        cmp al, 48                              ; checking if there is a leading zero, 48 in ascii is '0'
        je printLowerCharOfValue
        
        mov [ebp-4], eax
        mov edx, ebp
        sub edx, 4
        pushad
        push edx
        call printf
        add esp, 4
        push dword [ebp+8]
        call fflush
        add esp, 4
        popad
        jmp lastpopAndPrintLoopEnd
        
        printLowerCharOfValue:                  ; if there is a leading zero, print the value after the leading zero
            shr eax, 8
            mov [ebp-4], eax
            mov edx, ebp
            sub edx, 4
            pushad
            push edx
            call printf
            add esp, 4
            push dword [ebp+8]
            call fflush
            add esp, 4
            popad
        
        lastpopAndPrintLoopEnd:
            mov esp, ebp
            ret

;~~~~~~~duplicate~~~~~~~;

duplicate:
    mov ebp, esp                                ; set up ebp and esp
    inc dword [countMoves]               ; increment by 1 the operations counter

    pushRegs
    call popNode
    popRegs
    cmp eax, 0                                  ; pop node from the top of the operand stack
    je duplicateEnd

    mov ebx, eax                                ; ebx = the popped node from the top of the stack
    pushRegs
    push ebx
    call pushNodeToOperandStack                 ; push the element we just popped to the stack
    add esp, 4
    popRegs
    
    pushRegs
    call createNodeOnOperandStack               ; create a new node on operand stack for the duplicate element that we need to push
    popRegs
    cmp eax, 0                                  ; failed to create a node on operand stack
    je duplicateEnd

    duplicateLoop:
        mov edx, eax                            ; edx = address of the new node, we want to put there the duplicate value
        mov eax, [ebx + DATA]                   ; eax = value of the popped node that we want to duplicate
        mov [edx + DATA], eax                   ; copy to edx the value we want to duplicate
        
        mov ebx, [ebx + NEXT]                   ; ebx = next node of the element
        cmp ebx, 0
        je duplicateEndDebug
        
        pushRegs
        call createNode                         ; create new node and copy the value we want to duplicate into it
        popRegs
        mov [edx + NEXT], eax
        mov edx, eax
        jmp duplicateLoop
    
    duplicateEndDebug:
        endOperationDebugMode duplicateEnd
    duplicateEnd:
        mov esp, ebp
        ret

;~~~~~~~bitwise AND~~~~~~~;


bitwiseAnd:                                     ; X&Y, X = top of the operand stack, Y = the element next to X in the operand stack.
    mov ebp, esp                                ; set up ebp and esp
    inc dword [countMoves]               ; increment by 1 the operations counter

    push ebp                                    ; backup ebp pointer
    call popTwoNodesFromStack                   ; pop two elements from the top of the stack for bitwise and between them
    pop ebp                                     ; restore ebp pointer
    cmp eax, 0                                  ; eax saves the second element and if its 0 so there are no 2 values to bitwise and between them so we finish
    je bitwiseAndEnd

    mov ecx, eax                                ; ecx saves the second element
    mov [node1], ecx                            ; updates the second node we have to free from stack, ecx = Y
    mov [node2], ebx                            ; updates the first node we have to free from stack, ebx = X

    pushRegs
    call createNodeOnOperandStack               ; create a new node on operand stack for the bitwise and element that we need to push
    popRegs
    bitwiseAndLoop:                             ; eax = address of the new node, we want to put there the bitwise and value, edx = temp register
        mov dl, [ebx + DATA]                    ; dl = X, first node of X           
        mov [eax + DATA], dl                    ; eax = X, first node of X
        mov dl, [ecx + DATA]                    ; dl = Y, first node of Y
        and [eax + DATA], dl                    ; eax = X&Y

        mov ebx, [ebx + NEXT]                   ; updates ebx to the next node in X
        mov ecx, [ecx + NEXT]                   ; updates ecx to the next node in Y
        
        cmp ebx, 0                              ; ebx saves the next node of the first element and if its 0 so there are no more nodes to bitwise and
        je bitwiseAndEndFree

        cmp ecx, 0                              ; ecx saves the next node of the second element and if its 0 so there are no more nodes to bitwise and
        je bitwiseAndEndFree

        mov edx, eax                            ; create new node and copy the value of the bitwise and into it
        pushRegs
        call createNode
        popRegs
        mov [edx + NEXT], eax
        jmp bitwiseAndLoop
    
    bitwiseAndEndFree:                           
        pushad
        push dword [node1]                      ; free the second node from the stack, node1 = Y
        call freeLinkedList
        add esp, 4
        popad

        pushad
        push dword [node2]                      ; free the furst node from the stack, node2 = X
        call freeLinkedList
        add esp, 4
        popad
    
    leadloop:                                   ; fix leading zeros
        mov eax,dword [stack]                   ; eax=address of stack pointer
        mov eax, dword[eax]                     ; eax= address of first node
        mov [tempNode], dword eax               ; tempnode=eax
        cmp dword [eax+NEXT], 0                 ; check if there are more nodes
        je oneLink
        mov eax, dword [eax+NEXT]               ; dont want to remove the first link 
        .Zloop:
        mov ebx, dword [eax+NEXT]               ; ebx=address of node
        cmp ebx, 0
        je .checkIfZero
        mov [tempNode], dword eax
        mov eax, ebx
        jmp .Zloop
        .checkIfZero:
        cmp byte [eax], 0                       ; check if node data=0
        je .removeLink
        jmp oneLink
        .removeLink:                            ; remove a node  that is zero
        pushRegs             
        push dword eax
        call free
        add esp, 4
        popRegs
        mov eax, [tempNode]
        mov dword [eax+1], 0
        jmp leadloop

    oneLink:
    endOperationDebugMode bitwiseAndEnd
    bitwiseAndEnd:
        mov esp, ebp
        ret    

;~~~~~~~bitwise OR~~~~~~~;

bitwiseOr:                                      ; X|Y, X = top of the operand stack, Y = the element next to X in the operand stack.
    mov ebp, esp                                ; set up ebp and esp
    inc dword [countMoves]               ; increment by 1 the operations counter

    push ebp                                    ; backup ebp pointer
    call popTwoNodesFromStack                   ; pop two elements from the top of the stack for bitwise or between them
    pop ebp                                     ; restore ebp pointer
    cmp eax, 0                                  ; eax saves the second element and if its 0 so there are no 2 values to bitwise or between them so we finish
    je bitwiseOrEnd

    mov ecx, eax                                ; ecx saves the second element
    mov [node1], ecx                            ; updates the second node we have to free from stack, ecx = Y
    mov [node2], ebx                            ; updates the first node we have to free from stack, ebx = X

    pushRegs
    call createNodeOnOperandStack               ; create a new node on operand stack for the bitwise or element that we need to push
    popRegs
    bitwiseOrLoop:                              ; eax = address of the new node, we want to put there the bitwise and value, edx = temp register
        mov dl, [ebx + DATA]                    ; dl = X, first node of X  
        mov [eax + DATA], dl                    ; eax = X, first node of X
        mov dl, [ecx + DATA]                    ; dl = Y, first node of Y
        or [eax + DATA], dl                     ; eax = X|Y

        mov ebx, [ebx + NEXT]                   ; updates ebx to the next node in X
        mov ecx, [ecx + NEXT]                   ; updates ecx to the next node in Y
        
        cmp ebx, 0                              ; ebx saves the next node of the first element and if its 0 so there are no more nodes to bitwise or
        je bitwiseOrFinalLoop

        cmp ecx, 0                              ; ecx saves the next node of the second element and if its 0 so there are no more nodes to bitwise or
        je FlipRegisters

        mov edx, eax                            ; create new node and copy the value of the bitwise or into it
        pushRegs
        call createNode
        popRegs
        mov [edx + NEXT], eax
        jmp bitwiseOrLoop

    FlipRegisters:                              ; swap next nodes of X and Y elements, ebx = Y, ecx = X
        mov edx, ebx
        mov ebx, ecx
        mov ecx, edx

    bitwiseOrFinalLoop:
        cmp ecx, 0                              ; ecx saves the next node of the first element and if its 0 so there are no more nodes to bitwise or
        je bitwiseOrEndFree

        mov edx, eax
        pushRegs
        call createNode
        popRegs
        mov [edx + NEXT], eax
        
        mov dl, [ecx + DATA]
        mov [eax + DATA], dl
        
        mov ecx, [ecx + NEXT]
        jmp bitwiseOrFinalLoop
    
    bitwiseOrEndFree:
        pushad
        push dword [node1]                      ; free the second node from the stack, node1 = Y
        call freeLinkedList
        add esp, 4
        popad

        pushad
        push dword [node2]                       ; free the furst node from the stack, node2 = X
        call freeLinkedList
        add esp, 4
        popad
        endOperationDebugMode bitwiseOrEnd
    bitwiseOrEnd:
        mov esp, ebp
        ret

;~~~~~~~number of hexadecimal digits~~~~~~~;

numDigits:                                              ; number of hexadecimal digits of a input number
    mov ebp, esp                                        ; set up ebp and esp
    inc dword [countMoves]                       ; increment by 1 the operations counter
    mov [countZeros], dword 0

    call popNode
    cmp eax, 0                                          ; pop node from the top of the operand stack
    je numDigitsEnd
    mov edx, eax                                        ; edx = the popped node from the top of the stack, just for backup
    mov ebx, eax                                        ; ebx = the popped node from the top of the stack, for numDigitsLoop

    mov [node1], edx                                    ; updates the hexa number node we have to free from stack                   
    pushRegs
    call createNodeOnOperandStack                       ; create a new node on operand stack for the numDigits element that we need to push
    popRegs

    numDigitsLoop:
        cmp dword [ebx + NEXT], 0                       ; checks if the next node is 0 so we finish counting th number of digits
        je numDigitsLastLoop
        
        pushRegs
        call usafeCreateNode             ; create a new node and push it to stack, address is saved on eax
        popRegs
        mov byte [eax + DATA], 2
        mov byte [sumDigits], 1                         ; turn on the sumDigits flag so it won't print it in debug mode
        pushRegs
        call addition
        popRegs
        mov byte [sumDigits], 0                         ; turn off the sumDigits flag
        dec dword [countMoves]                   ; fix operation counter and decrease by 1 because of addition

        mov ebx, [ebx + NEXT]                           ; next node to count his digit in the numDigits counter
        cmp ebx, 0
        jne numDigitsLoop

    numDigitsLastLoop:                                  ; if the next node is \n so we finish counting the number of digits
        cmp byte [ebx + DATA], 0
        jne continue2
        inc dword [countZeros]
        continue2:
        cmp byte [ebx + DATA], 0x10
        jb addOneToNumberOfHexDigits

        pushRegs
        call usafeCreateNode             ; create a new node and push it to stack, address is saved on eax
        popRegs
        mov byte [eax + DATA], 1
        mov byte [sumDigits], 1                         ; turn on the sumDigits flag so it won't print it in debug mode
        pushRegs
        call addition
        popRegs
        mov byte [sumDigits], 0                         ; turn off the sumDigits flag
        dec dword [countMoves]                   ; fix operation counter and decrease by 1 because of addition
        
        addOneToNumberOfHexDigits:
            pushRegs
            call usafeCreateNode         ; create a new node and push it to stack, address is saved on eax
            popRegs
            mov byte [eax + DATA], 1
            mov byte [sumDigits], 1                     ;turn on the flag so it won't print in debug the sum
            pushRegs
            call addition
            popRegs
            mov byte [sumDigits], 0                     ; turn off the sumDigits flag
            dec dword [countMoves]               ; fix operation counter and decrease by 1 because of addition

        pushad
        push edx
        call freeLinkedList                             ; free the popped number from the linked list saved in edx
        add esp, 4
        popad
    endOperationDebugMode numDigitsEnd
    numDigitsEnd:
        mov esp, ebp
        ret


; gets the number of bytes to read from the input buffer, that represents a hex number in string.
; convert the string to its numeric value and push it to the operand stack.
pushByteToHexa:
    mov ebp, esp

    cmp byte [debug], 1
    jne notDebug
    push dword in
    push dword format_string
    push dword [stderr]
    call fprintf
    add esp, 12
    push dword [stderr]
    call fflush
    add esp, 4

    notDebug:
    pushRegs
    call createNodeOnOperandStack
    popRegs
    cmp eax, 0                                  ; failed to create a node on operand stack
    je pushByteToHexaEnd

    mov edx, eax                                ; edx = dddress of the new node
    
    pushByteToHexaStart:
    pushRegs
    call countLeadingZeros                ; eax = number of leading zeros
    popRegs

    convertInputToNodes:
    mov ecx, [ebp+4]                            ; length of the input string hexa number
    mov ebx, eax                                ; ebx = number of leading zeros
    sub ecx, ebx                                ; ecx = string hexa number length - leading zeros
                 
    ; ecx = number of remaining chars to read
    ; edx = address of current node

    cmp ecx, 0                                  ; if the number is 0 we finished with byte to hexa
    je pushByteToHexaEnd

    convertInputLoop:
        
        cmp ecx, 1
        je convertOneCharFromInput              ; if only 1 char needs to be read

        
        push eax                                ; else, we have to read 2 chars or more
        pushRegs
        push word [in + ebx + ecx - 2]
        call HexaToByte
        add esp, 2                              ; remove pushed word from stack
        popRegs
        mov [edx + DATA], al
        pop eax

        ; if there are no more chars to read, jump to the end of convert
        sub ecx, 2
        cmp ecx, 0
        jz pushByteToHexaEnd

        pushRegs
        call createNode
        popRegs

        mov [edx + NEXT], eax                   ; set next node field of the previous node to point to the new one
        mov edx, eax
        jmp convertInputLoop

    convertOneCharFromInput:
        mov bx, [in + ebx]
        shl bx, 8                               ; fill with zeros
        push eax
        pushRegs
        push word bx
        call HexaToByte
        add esp, 2                              ; remove pushed word from stack
        popRegs
        mov [edx + DATA], al
        pop eax

    pushByteToHexaEnd:
        ret
        
; returns the number of leading '0' characters in input buffer
countLeadingZeros:
    mov ebx, in
    mov eax, 0                                  ; leading zeros counter
    countLeadingZerosLoop:
        cmp byte [ebx + eax], 48                ; '0' in ascii
        jne endCountLeadingZeros
        inc eax
        jmp countLeadingZerosLoop
    endCountLeadingZeros:
        ret

; gets a word representing 2 hexadecimal digits and return the value they represent.
HexaToByte:
    mov ebp, esp

    mov dx, [ebp+4]
    mov al, [ebp+4]
    cmp al ,byte 'A'                            ; compare to 'A' 
	jge above_A
	sub  al,byte 48                             ; is a digit between 0-9
	jmp finishA
    above_A:
	sub al, byte 55                             ; is a digit between A-F
    finishA:

    shr dx, 8                                   ; dl = dh, dh = 0
    cmp dl, 0                                   ; If it's a null terminator byte, ignore it
    jz returnHexaValue                          ; al already contains the value

    shl al, 4                                   ; multiply value by 0x10
    mov cl, al
    mov al,dl
    cmp al ,byte 'A'                            ; compare to 'A' 
	jge above_A2
	sub  al,byte 48                             ; is a digit between 0-9
	jmp finishA2
    above_A2:
	sub al, byte 55                             ; is a digit between A-F
    finishA2:

    add al, cl

    returnHexaValue:
        mov esp, ebp
        ret                                     ; stores the result in al

; gets a byte of data and return a hexadecimal digit string representing it.
byteToHexa:
    mov ebp, esp

    mov dx, [esp+4]
    mov al, [ebp+4]
    and al, 0x0F
    cmp al, 0xA                                 ; compare to a
    jl not_A
    add al, 0x7                                 ; if A-F need to add 48+7
    not_A:
    add al, 0x30                                ; between 0-9 need to add only 48
    
    mov ch, al

    shr dx, 4
    mov al,dl
    and al, 0x0F
    cmp al, 0xA
    jl not_A2
    add al, 0x7
    not_A2:
    add al, 0x30

    mov cl, al

    mov ax, cx
    mov esp, ebp
    ret                                         ; stores the result in al

; allocate memory for a node and put its address in eax.
createNode:
    push dword 1                                ;size
    push dword 5                                ;4 for pointer and 1 for value, total of 5
    call calloc
    add esp, 8
    ret

; gets an address of the starting node of the list and free the memory allocated for all of the nodes in including it.
freeLinkedList:
    mov eax, [esp+4]

    freeNextNode:
    mov ebx, [eax + NEXT]
    push dword eax
    call free
    add esp, 4
    mov eax, ebx
    cmp eax, 0
    jnz freeNextNode
    ret

; create a new node and try to push it to the end of operand stack.
createNodeOnOperandStack:
    mov ebp, esp

    pushRegs
    call createNode
    popRegs
    mov edx, eax                                ; edx = Address of the new node
    
    pushad
    push edx
    call pushNodeToOperandStack
    add esp, 4
    popad
    
    cmp eax, 0                                  ; failed to push to operand stack
    jz createNodeOnOperandStackFailure
    mov eax, edx
    ret

    createNodeOnOperandStackFailure:
    push dword edx
    call free
    add esp, 4
    mov eax, 0
    ret                                         ; returns the address of the new node if success and 0 otherwise

; create a new node and push it to the end of operand stack. without checking operand stack
usafeCreateNode:
    pushRegs
    call createNode
    popRegs
    mov edx, eax                                ; edx = Address of the new node
    
    pushad
    push edx
    call unsafePushNode
    add esp, 4
    popad    
    mov eax, edx
    ret                                         ; used only for numDigits

; gets an address of a node and push it to the end of the operand stack.
pushNodeToOperandStack:
    
    mov eax, 0                                  ; reset register
    mov al, [countItems]                        ; Check if stack is full of elements and can't push one more
    cmp [stackSize], al
    jne unsafePushNode
    push stack_overflow_error
    call printf
    add esp, 4
    push dword [stdout]
    call fflush
    add esp, 4
    mov eax, 0
    ret                                         ; returns 1 if success and 0 otherwise

    
    unsafePushNode:                             ; push to stack
    mov eax, 0                                  ; reset register
    mov al, [countItems]

    mov ebx, [esp+4]
    mov ecx, [stack]
    mov [ecx + 4 * eax], ebx
    inc byte [countItems]
    mov eax, 1
    ret

; pop 2 nodes from the stack and place the address of top in ebx and the second in eax.
popTwoNodesFromStack:
    mov ebp, esp

    pushRegs
    call popNode
    popRegs
    cmp eax, 0
    je popTwoNodesFromStackEnd                  ; if there is a fail
    mov ebx, eax

    pushRegs
    call popNode
    popRegs
    cmp eax, 0
    jne popTwoNodesFromStackEnd                 ; if its successed to pop

    ; if only second pop failed, push back only the first node
    pushad
    push ebx
    call pushNodeToOperandStack
    add esp, 4
    popad

    mov eax, 0
    ret

    popTwoNodesFromStackEnd:
        mov esp, ebp
        ret

; pop node from stack
popNode:
    mov edx, 0 									; Reset register
    mov dl, [countItems]						; check if stack is empty 
    cmp edx, 0
    je popNodeError

    dec edx 									; index starts at 0
    mov ebx, [stack]							; ebx=address of stack
    mov ecx, [ebx + edx * 4] 					; ecx = address of last input node
    
    mov dword [ebx + edx * 4] , 0				; put 0 insted of the address- null last node
    
    mov eax, ecx								; eax=ecx=address of first node
    dec byte [countItems]
    jmp popNodeEnd

    popNodeError:								; if stack is empty print error
    mov eax, 0  
    push stack_empty
    call printf
    add esp, 4
    push dword [stdout]
    call fflush
    add esp, 4

    popNodeEnd:
    ret
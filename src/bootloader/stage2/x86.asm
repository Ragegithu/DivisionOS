BITS 16


;void _cdecl x86_div64_32(uint64_t dividend, uint32_t divisor, uint64_t* qoutientOut, uint_t32* remainderOut)

global _x86_div64_32:
_x86_div64_32:
    push bp
    mov bp,sp
    push bx    
    ;divide upper 32 bits
    mov eax, [bp + 8]
    mov ecx, [bp + 12]

    ;clear and divide upper 32 bits (since its little endian
    xor edx, edx
    div ecx
    ;store the bits
    mov bx, [bp + 16]
    mov [bx + 4], eax

    ;divide lower 32 bits
    mov eax, [bp+4]

    div ecx

    ;store results
    mov [bx], eax
    mov bx, [bp + 18]
    mov [bx], edx

    pop bx


    mov sp,bp
    pop bp
    ret


global __U4D
__U4D:
    ; DX:AX = dividend, CX:BX = divisor
    ; returns DX:AX = quotient, CX:BX = remainder

    ; pack DX:AX into edx, then move to eax
    shl edx, 16
    mov dx, ax
    mov eax, edx

    ; pack CX:BX into ecx
    shl ecx, 16
    mov cx, bx

    ; do the division
    xor edx, edx
    div ecx             ; eax = quotient, edx = remainder

    ; split quotient into DX:AX
    push edx            ; save remainder
    mov edx, eax
    shr edx, 16         ; dx = high word, ax = low word

    ; split remainder into CX:BX
    pop ecx
    mov ebx, ecx
    shr ecx, 16         ; cx = high word, bx = low word

    ret


section _TEXT class=CODE

global _x86_Video_WriteCharTeletype
_x86_Video_WriteCharTeletype:
    push bp
    mov bp,sp
    
    mov ah, 0x0e
    mov al, [bp + 4]
    mov bh, [bp + 6]

    int 0x10

    pop bx

    mov sp,bp
    pop bp
    ret

global _diskRead_asm
_diskRead_asm:
    push bp
    mov bp, sp
    mov ah, 0x02
    mov al, [bp + 4]
    mov ch, [bp + 6]
    mov cl, [bp + 8]
    mov dh, [bp + 10]
    mov dl, [bp + 12]
    mov bx, [bp + 14]

    push ds
    pop es

    int 0x13

    jnc .done
    mov al, ah      ; error code into AL (low byte of return)
    xor ah, ah      ; zero AH so it's a clean small int
    pop bp
    ret

.done:
    mov ax, 1
    pop bp
    ret
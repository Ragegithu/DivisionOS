BITS 16

section _ENTRY class=CODE

extern _cstart_
global entry

entry:
    cli
    mov ax,ds
    mov ss, ax
    mov sp, 0
    mov bp, sp
    sti

    ;expect boot drive in dl whatever the fuck that means
    xor dh,dh 
    push dx
    call _cstart_ ;kernel is loaded at 0x2c000

    ;do protected mode magic here



    cli
    call EnableA20 ; 2 enable a20 gate
    call loadGDT ; 3 load global discriptor table
    ; 4 set protected mode enabled bit 
    mov eax, cr0
    or al, 1
    mov cr0, eax

    ;5 far jump
    jmp dword 08h:.pmode32

    

.pmode32:
    [bits 32]
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov esp, 0x90000

    mov byte [0xB8000], 'X'     ; debug — remove later
    mov byte [0xB8001], 0x0F

    mov eax, 0x2c000
    jmp eax

;load gdt
loadGDT:
    [bits 16]
    lgdt [g_GDTDesc]
    ret


EnableA20:
    [bits 16]
    call A20WaitInput
    mov al, KbdControllerDisableKbd
    out KbdControllerCommandPort, al

    ;read control output port
    call A20WaitInput
    mov al, KbdControllerReadCtrlOutputPort
    out KbdControllerCommandPort, al

    call A20WaitOutput
    in al, KbdControllerDataPort
    push eax

    call A20WaitInput
    mov al, KbdControllerWriteCtrlOutputPort
    out KbdControllerCommandPort, al

    call A20WaitInput
    pop eax
    or al, 2
    out KbdControllerDataPort, al

    call A20WaitInput
    mov al, KbdControllerEnablekbd
    out KbdControllerCommandPort, al

    call A20WaitInput
    ret



    
A20WaitInput:
    [bits 16]
    ;wait for status bit 2 to be 0
    in al, KbdControllerCommandPort
    test al, 2
    jnz A20WaitInput
    ret

A20WaitOutput:
    [bits 16]
    ;status bit 1 has to be 1 so we wait
    in al, KbdControllerCommandPort
    test al, 1
    jz A20WaitOutput
    ret

;switch to protected mode shit above




KbdControllerDataPort equ 0x60
KbdControllerCommandPort equ 0x64

KbdControllerDisableKbd equ 0xad
KbdControllerEnablekbd equ 0xae

KbdControllerReadCtrlOutputPort equ 0xd0
KbdControllerWriteCtrlOutputPort equ 0xd1


g_GDT:
    dq 0

    ;code segment
    dw 0xffff
    dw 0
    db 0
    db 10011010b
    db 11001111b
    db 0

    ;data segment
    dw 0xffff
    dw 0
    db 0
    db 10010010b
    db 11001111b
    db 0


    ;16b code segment
    dw 0xffff
    dw 0
    db 0
    db 10011010b
    db 00001111b
    db 0

    ;16b data segment
    dw 0xffff
    dw 0
    db 0
    db 10010010b
    db 00001111b
    db 0

g_GDTDesc: 
    dw g_GDTDesc - g_GDT - 1
    dd g_GDT


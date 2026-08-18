[bits 32]

global _irq1_handler
global _outb
global _inb

extern _keyboard_handler

_irq1_handler:
    pusha ;save all regs
    call _keyboard_handler ;call the c func
    popa ; restore all regs
    iret ;retrun from interrupt

_outb:
    movzx edx, word [esp + 4]   ; port
    movzx eax, byte [esp + 8]   ; value
    out dx, al
    ret

_inb:
    movzx edx, word [esp + 4]   ; port
    xor eax, eax
    in al, dx
    ret
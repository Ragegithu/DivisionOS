#include "idt.h"

static IDTEntry idt[256];
static IDTPointer idtp;

void idt_set_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags)
{
    idt[num].base_low  = (base & 0xFFFF);
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector  = selector;
    idt[num].reserved  = 0;
    idt[num].flags     = flags;
}

void idt_init(void)
{
    idtp.limit = (sizeof(IDTEntry) * 256) - 1;
    idtp.base  = (uint32_t)&idt;
    __asm {lidt fword ptr [idtp]};
}
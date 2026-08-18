#ifndef IDT_H
#define IDT_H

#include "stdint.h"

#pragma pack (push, 1)
typedef struct
{
    uint16_t base_low;
    uint16_t selector;
    uint8_t reserved;
    uint8_t flags;
    uint16_t base_high;
}IDTEntry;
#pragma pack (pop)

#pragma pack (push,1)
typedef struct
{
    uint16_t limit;
    uint32_t base;
} IDTPointer;
#pragma pack (pop)


void idt_set_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags);
void idt_init(void);

#endif
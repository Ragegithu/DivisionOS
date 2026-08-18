// x86.h
#ifndef X86_H
#define X86_H

#include "stdint.h"

extern void _cdecl outb(uint16_t port, uint8_t value);
extern uint8_t _cdecl inb(uint16_t port);

#endif
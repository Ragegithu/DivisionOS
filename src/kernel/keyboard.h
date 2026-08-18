#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "stdint.h"

void init_keyboard(void);

extern void _cdecl keyboard_handler(void);

char getc (void);
void gets(char* buffer, int max);

#endif
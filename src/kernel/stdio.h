#pragma once

void putc(char c);
void puts(const char* str);
void _cdecl printf(const char* fmt, ...);

static inline int strcmp(const char* a, const char* b)
{
    while(*a && *b && *a == *b)
    {
        a++;
        b++;
    }
    return *a - *b;
}
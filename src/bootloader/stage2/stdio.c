#include "stdio.h"
#include "stdint.h"
#include "x86.h"


void putc(char c)
{
    x86_Video_WriteCharTeletype(c, 0);
}
void puts(const char* str)
{
    while(*str)
    {
        putc(*str);
        str++;
    }
}

#define PRINTF_STATE_NORMAL 0
#define PRINTF_STATE_LENGTH 1
#define PRINTF_STATE_LENGTH_SHORT 2
#define PRINTF_STATE_LENGTH_LONG 3
#define PRINTF_STATE_SPECIFIER 4

#define PRINTF_LENGTH_DEFAULT 0
#define PRINTF_LENGTH_SHORT_SHORT 1
#define PRINTF_LENGTH_SHORT 2
#define PRINTF_LENGTH_LONG 3
#define PRINTF_LENGTH_LONG_LONG 4

int* printf_number(int* argp, int length, bool signed_, int radix);

void _cdecl printf(const char* fmt, ...)
{
    int * argp = (int*)&fmt; 
    int state = PRINTF_STATE_NORMAL;
    int length = PRINTF_LENGTH_DEFAULT;
    int radix = 10;
    bool signed_ = false;

    argp++;
    while(*fmt)
    {
        switch (state)
        {
            case PRINTF_STATE_NORMAL:
                switch (*fmt)
                {
                    case '%': state = PRINTF_STATE_LENGTH;
                        break;
                    default: putc(*fmt);
                        break;
                }
                break;
            
            case PRINTF_STATE_LENGTH:
                switch (*fmt)
                {
                    case 'h': length = PRINTF_LENGTH_SHORT;
                        state = PRINTF_STATE_LENGTH_SHORT;
                        break;
                    case 'l': length = PRINTF_LENGTH_LONG;
                        state = PRINTF_STATE_LENGTH_LONG;
                        break;
                    default: goto PRINTF_STATE_SPECIFIER_;
                }
                break;

            case PRINTF_STATE_LENGTH_SHORT:
                if(*fmt == 'h')
                {
                    length = PRINTF_LENGTH_SHORT_SHORT;
                    state = PRINTF_STATE_SPECIFIER;
                }
                else goto PRINTF_STATE_SPECIFIER_;
                break;
            case PRINTF_STATE_LENGTH_LONG:
                if(*fmt == 'l')
                {
                    length = PRINTF_LENGTH_LONG_LONG;
                    state = PRINTF_STATE_SPECIFIER;
                }
                else goto PRINTF_STATE_SPECIFIER_;
                break;
            case PRINTF_STATE_SPECIFIER:
                PRINTF_STATE_SPECIFIER_:
                    switch (*fmt)
                    {
                    case 'c': 
                        putc((char)* argp);
                        argp++;
                        break;
                    
                    case 's': 
                        puts(*(char**) argp);
                        argp++;
                        break;
                    case '%':
                        putc('%');
                        break;
                    case 'd':
                    case 'i':
                        radix = 10; signed_ = true;
                        argp = printf_number(argp, length, signed_, radix);
                        break;
                    case 'u':
                        radix = 10; signed_ = false;
                        argp = printf_number(argp, length, signed_, radix);
                        break;
                    case 'X':
                    case 'x':
                    case 'p':
                        radix = 16; signed_ = false;
                        argp = printf_number(argp, length, signed_, radix);
                        break;
                    case 'o':
                        radix = 8; signed_ = false;
                        argp = printf_number(argp, length, signed_, radix);
                        break;
                    
                    default:
                        break;
                    }
                    state = PRINTF_STATE_NORMAL;
                    length = PRINTF_LENGTH_DEFAULT;
                    radix = 10;
                    signed_ = false;
                    break;
        
        }
        fmt++;
    }
}

const char g_HexChar[] = "0123456789abcdef";

int* printf_number(int* argp, int length, bool signed_, int radix)
{
    char buffer[32];
    unsigned long long number;
    int number_sign = 1;
    int pos =0;

    switch (length)
    {
        case PRINTF_LENGTH_SHORT_SHORT:
        case PRINTF_LENGTH_SHORT:
        case PRINTF_LENGTH_DEFAULT:
            if(signed_)
            {
                int n = *argp;
                if(n < 0)
                {
                    n = -n;
                    number_sign = -1;
                }
                number = (unsigned long long)n;
            }
            else
            {
                number = *(unsigned int*)argp;
            }
            argp++;
            break;

        case PRINTF_LENGTH_LONG:
            if(signed_)
            {
                long int n = *(long int*)argp;
                if(n < 0)
                {
                    n = -n;
                    number_sign = -1;
                }
                number = (unsigned long long)n;
            }
            else
            {
                number = *(unsigned long int*)argp;
            }
            argp += 2;
            break;

        case PRINTF_LENGTH_LONG_LONG:
            if(signed_)
            {
                long long int n = *(long long int*)argp;
                if(n < 0)
                {
                    n = -n;
                    number_sign = -1;
                }
                number = (unsigned long long)n;
            }
            else
            {
                number = *(unsigned long long int*)argp;
            }
            argp += 4;
            break;
    }
    //convert number to ascii
    do
    {
        uint32_t rem;
        x86_div64_32(number , radix, &number, &rem);
        //number = number / radix;
        buffer[pos++] = g_HexChar[rem];
    }while(number > 0);

    //add sign
    if(signed_ && number_sign < 0)
    {
        buffer[pos++] = '-';
    }

    //print in reverse
    while (--pos >= 0)
    {
        putc(buffer[pos]);
    }
    return argp;
}
#include "stdio.h"
#include "stdint.h"
#include "x86.h"

#define VGA_WIDTH  80
#define VGA_HEIGHT 25


static uint16_t* const VGA = (uint16_t*)0xB8000;
static int cursor_x = 0;
static int cursor_y = 0;

static uint8_t current_attr = 0x0f; // default white on black


void set_color(uint8_t fg, uint8_t bg)
{
    current_attr = (bg << 4) | (fg & 0x0F);
}

void update_hw_cursor(int row, int col)
{
    uint16_t pos = row * 80 + col;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xff));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xff));
}

void set_cursor(int x, int y)
{
    cursor_x = x;
    cursor_y = y;
}

void get_cursor(int* x, int* y)
{
    *x = cursor_x;
    *y = cursor_y;
}

void clearf(void)
{
    int i;
    for(i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        VGA[i] = (uint16_t)(current_attr << 8);   // blank with white-on-black attribute
    cursor_x = 0;
    cursor_y = 0;
}

void putc(char c)
{
    if(c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
    }
    else if(c == '\r')
    {
        cursor_x = 0;
    }
    else if(c == '\b')
    {
        if(cursor_x > 0)
            cursor_x--;
    }
    else
    {
        VGA[cursor_y * VGA_WIDTH + cursor_x] = (uint16_t)((current_attr << 8) | (uint8_t)c);
        cursor_x++;
        
        if(cursor_x >= VGA_WIDTH)
        {
            cursor_x = 0;
            cursor_y++;
        }
    }

    if(cursor_y >= VGA_HEIGHT)
    {
        int i;
        int j;
        for(i = 0; i < VGA_WIDTH * (VGA_HEIGHT - 1); i++)
            VGA[i] = VGA[i + VGA_WIDTH];
        for(j = VGA_WIDTH * (VGA_HEIGHT - 1); j < VGA_WIDTH * VGA_HEIGHT; j++)
            VGA[j] = (uint16_t)(current_attr << 8);
        cursor_y = VGA_HEIGHT - 1;
    }


    update_hw_cursor(cursor_y,cursor_x);
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

void printf(const char* fmt, ...)
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
    uint32_t num32 = (uint32_t)number;  // safe: no number you're printing needs 64-bit
    do
    {
        buffer[pos++] = g_HexChar[num32 % radix];
        num32 /= radix;
    } while (num32 > 0);

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
#include "idt.h"
#include "keyboard.h"
#include "x86.h"

extern void _cdecl irq1_handler(void);


static char key_buffer[256];
static int buf_pos;

static char scancode_table[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


void init_keyboard(void)
{
    outb(0x20,0x11);
    outb(0xA0,0x11);

    outb(0x21,0x20);
    outb(0xA1,0x28);

    outb(0x21,0x04);
    outb(0xA1,0x02);

    outb(0x21,0x01);
    outb(0xA1,0x01);

    outb(0x21,0xFD);
    outb(0xA1,0xFF);

    idt_set_gate(0x21, (uint32_t)irq1_handler, 0x08,0x8E);
    idt_init();
    __asm { sti }
}

void keyboard_handler(void)
{
    uint8_t scancode = inb(0x60);
    char c;
    if(scancode < 128)
    {
        c = scancode_table[scancode];
        if(c != 0)
        {
            key_buffer[buf_pos] = c;
            buf_pos++;
        }
    }
    outb(0x20,0x20);
}

char getc(void)
{
    char c;

    while(buf_pos == 0){/*wait lol*/}

    buf_pos--;
    c = key_buffer[buf_pos];
    return c;
}


void gets(char* buffer, int max)
{
    int i =0;
    char c;
    while(i < max -1)
    {
        c = getc();
        if( c == '\n' || c == '\r')
            break;
        else if(c == '\b')
        {
            if(i > 0)
            {
                i--;
                putc('\b');
                putc(' ');
                putc('\b');
            }
        }
        else
        {
            buffer[i++] = c;
            putc(c);
        }
    }
    buffer[i] = '\0';
    putc('\n');
}
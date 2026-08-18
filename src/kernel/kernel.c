#include "stdint.h"
#include "stdio.h"
#include "keyboard.h"
#include "shell.h"

void kmain(void)
{
    //boot up and initialize keyboard and shell
    cmd_name();
    init_keyboard();
    
    char input[256];
    parsed_cmd cmd; //defined in shell.h


    while(1){
        printf(">");
        gets(input, 256);
        parse_command(input, &cmd);

        if(cmd.argc == 0) continue;

        if(strcmp(cmd.argv[0], "help") == 0)
        {
            set_color(0x4,0x0);
            cmd_help();
            set_color(0xf,0x0);
        }
        else if(strcmp(cmd.argv[0], "clear") == 0)
        {
            cmd_clear();
            set_color(0xf,0x0);
        }
        else if(strcmp(cmd.argv[0], "name") == 0)
        {
            cmd_name();
            set_color(0xf,0x0);
        }
    }
    for(;;);
}
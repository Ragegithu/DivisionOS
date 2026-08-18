#include "shell.h"
#include "stdio.h"
#include "keyboard.h"

void parse_command(char* input, parsed_cmd* cmd)
{
    cmd->argc = 0;

    char* p = input;

    while(*p != '\0' && cmd->argc < MAX_ARGS)
    {
        while(*p == ' ') p++;
        if(*p == '\0') break;

        cmd->argv[cmd->argc++] = p;

        while(*p != ' ' && *p != '\0')
            p++;

        if(*p == ' ') *p = '\0';
    }
}

void cmd_help()
{
    printf("'fraid there isn't any help here\n");
}
void cmd_clear()
{
    clearf();
}
void cmd_name()
{
    const char* art[5];
    int i;

    art[0] = "####  ###  #   #  ###   ###   ###   ###  #   #      ###   ### ";
    art[1] = "#   #  #   #   #   #   #       #   #   # ##  #     #   # #   ";
    art[2] = "#   #  #    # #    #    ##     #   #   # # # #     #   #  ## ";
    art[3] = "#   #  #    # #    #      #    #   #   # #  ##     #   #    #";
    art[4] = "####  ###    #    ###  ###    ###   ###  #   #      ###  ### ";

    clearf();
    set_color(0x9, 0x0);

    for(i = 0; i < 5; i++)
    {
        set_cursor(9, 10 + i);   // (80-61)/2 = 9
        puts(art[i]);
    }

    set_color(0xF, 0x0);
    set_cursor(0, 22);
}
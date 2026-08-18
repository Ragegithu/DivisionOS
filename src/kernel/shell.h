#ifndef SHELL_H
#define SHELL_H

#define MAX_ARGS 16

typedef struct{
    char* argv[MAX_ARGS];
    int argc;
}parsed_cmd;

void parse_command(char* input, parsed_cmd* cmd);

//commands
void cmd_help();
void cmd_clear();
void cmd_name();

#endif
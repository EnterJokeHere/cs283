#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist){
    char *token;              
    char *commands[CMD_MAX];  // Array to hold command strings
    int cmd_count = 0;    // count cmds so that there's not more than the max size

    memset(clist, 0, sizeof(command_list_t));

    token = strtok(cmd_line, PIPE_STRING); // strtok is string to token
    while (token != NULL) {
        if (cmd_count >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }

        
        while (isspace((unsigned char)*token)){ 
            token++; // Trim leading/trailing spaces
        }
        char *end = token + strlen(token) - 1;
        while (end > token && isspace((unsigned char)*end)) {
            *end = '\0';
            end--;
        }

        commands[cmd_count++] = token;
        token = strtok(NULL, PIPE_STRING);
    }

    if (cmd_count == 0) { // No commands given check
        return WARN_NO_CMDS;
    }


    for (int i = 0; i < cmd_count; i++) {
        char *arg_pos = strchr(commands[i], SPACE_CHAR);
        if (arg_pos != NULL) {
            // Split into executable and arguments
            *arg_pos = '\0';
            strncpy(clist->commands[i].exe, commands[i], EXE_MAX - 1);
            strncpy(clist->commands[i].args, arg_pos + 1, ARG_MAX - 1);
        } else {
            // Only an executable, no arguments
            strncpy(clist->commands[i].exe, commands[i], EXE_MAX - 1);
        }
    }

    clist->num = cmd_count; //asigns a sub-value to the call for the cmd count
    return OK;
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"
#include "dragon.c"

#include <errno.h>



/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int exec_local_cmd_loop()
{
    char cmd_buff[SH_CMD_MAX];
    int rc = 0;
    int prev_rc = 0;
    cmd_buff_t cmd;

    // IMPLEMENT MAIN LOOP
    while (1)
    {
        // IMPLEMENT parsing input to cmd_buff_t *cmd_buff
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL){
            printf("\n");
            break;
        }
        // remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';

        if (strcmp(cmd_buff, EXIT_CMD) == 0){ // Check if input is "exit" (def in h file) to terminate the shell
            exit(0);
        }    
    

    

        rc = build_cmd_buff(cmd_buff, &cmd); // Pass command through build to make sure it's valid

        switch (rc) { //error cases
            case ERR_MEMORY:
                return rc;
            case WARN_NO_CMDS:
                printf(CMD_WARN_NO_CMD);
                continue;
            case ERR_TOO_MANY_COMMANDS:
                printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
                continue;
            default:
                break;
        }


        Built_In_Cmds bi_cmd = exec_built_in_cmd(&cmd);

        if (bi_cmd == BI_CMD_EXIT){
            break;
        }

        if (bi_cmd == BI_EXECUTED) {
            continue; // Prevent external execution attempt
        }        

        rc = exec_cmd(&cmd);
        if (rc != 0) {
        if (rc == EPERM) {
                printf("Not permitted\n");
            } else if (rc == ENOENT) {
                printf("Command not found in PATH\n");
            } else if (rc == EACCES) {
                printf("Permission denied\n");
            } else {
                printf("Command failed with exit code: %d\n", rc);
            }
        }
        prev_rc=rc;
    }

    //  IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
    // the cd command should chdir to the provided directory; if no directory is provided, do nothing

    //  IMPLEMENT if not built-in command, fork/exec as an external command
    // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"

    return OK;
}




int build_cmd_buff(char *cmd_line,  cmd_buff_t *cmd_buff){
    if (!cmd_line || !cmd_buff){
        return ERR_MEMORY;
    }

    cmd_buff->_cmd_buffer = strdup(cmd_line);
    if(!cmd_buff->_cmd_buffer){
        return ERR_MEMORY;
    }

    cmd_buff->argc=0;
    char *ptr = cmd_buff->_cmd_buffer; // Cmd pointer
    bool quotes=false;
    
    while (*ptr){
        while (isspace((unsigned char)*ptr) && !quotes){
            ptr++;
        } 

        if(*ptr == '\0'){
          break;  
        }

        if (*ptr == '"'){
            quotes = !quotes; //toggles quote mode bool
            ptr++;
            cmd_buff->argv;
            cmd_buff->argv[cmd_buff->argc++] = ptr;

            while (*ptr && (quotes || !isspace((unsigned char)*ptr))) {
                if (*ptr == '"') {
                    *ptr = '\0';
                    quotes=false;
                    break;
                }
                ptr++;
            }
        } else {
            cmd_buff->argv[cmd_buff->argc++] = ptr;
            while (*ptr && !isspace((unsigned char)*ptr)) {
                ptr++;
            }
        }

        if(*ptr){
            *ptr = '\0';
            ptr++;
        }
    }

    cmd_buff->argv[cmd_buff->argc] = NULL; // nullify arg list

    return(cmd_buff->argc > 0) ? OK : WARN_NO_CMDS;
}



Built_In_Cmds match_command(const char *input) {
    if (strcmp(input, "exit") == 0) {
        return BI_CMD_EXIT;
    }
    
    if (strcmp(input, "cd") == 0) {
        return BI_CMD_CD;
    }
    if (strcmp(input, "dragon") == 0) {
        return BI_CMD_DRAGON;
    }
    return BI_NOT_BI;
}


Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
    if (!cmd || cmd->argc == 0) {
        return BI_RC;
    }

    Built_In_Cmds cmd_type = match_command(cmd->argv[0]);

    switch (cmd_type) {
        case BI_CMD_EXIT:
            exit(0);   
        case BI_CMD_CD:
            if (cmd->argc > 1) {
                if (chdir(cmd->argv[1]) != 0) {
                    perror("cd");
                }
            }
            return BI_EXECUTED;
        case BI_CMD_DRAGON:
            print_dragon();
            return BI_EXECUTED;
        default:
            return BI_NOT_BI;
        }
}




int exec_cmd(cmd_buff_t *cmd) {
    pid_t pid = fork();
    if (pid == 0) {
        execvp(cmd->argv[0], cmd->argv);
        perror("execvp");
        exit(ERR_EXEC_CMD);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS (status);
    } else {
        perror("fork");
        return ERR_EXEC_CMD;
    }
}
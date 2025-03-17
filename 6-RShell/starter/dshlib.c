#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

/**** 
 **** FOR REMOTE SHELL USE YOUR SOLUTION FROM SHELL PART 3 HERE
 **** THE MAIN FUNCTION CALLS THIS ONE AS ITS ENTRY POINT TO
 **** EXECUTE THE SHELL LOCALLY
 ****
 */

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
 #include <stdlib.h>
 #include <stdio.h>
 #include <string.h>
 #include <ctype.h>
 #include <stdbool.h>
 #include <unistd.h>
 #include <fcntl.h>
 #include <sys/wait.h>
 #include "dshlib.h"
 //include "dragon.c"
 
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
     command_list_t clist;
 
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
      
          if (strlen(cmd_buff) == 0) {
             continue;
         }
 
          rc = build_cmd_list(cmd_buff, &clist); 
  
         if (rc == ERR_MEMORY) {
             return rc;
         }
         if (rc == WARN_NO_CMDS) {
             printf(CMD_WARN_NO_CMD);
             continue;
         }
         if (rc == ERR_TOO_MANY_COMMANDS) {
             printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
             continue;
         }
 
  
          rc = execute_pipeline(&clist);
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
   
      return OK;
  }
  
 
 int execute_pipeline(command_list_t *clist){
     int num_cmds = clist->num;
     int pipefds [2* (num_cmds - 1)];
 
     for (int i = 0; i < num_cmds - 1; i++) { //Make all of the pipes.
         if (pipe(pipefds + i * 2) == -1) {
             perror("pipe");
             return ERR_MEMORY;    
         }
     }
 
     //Fork processes for each command
     for (int i = 0; i < num_cmds; i++) {
         pid_t pid = fork();
         
         if (pid == 0) { // Child 
             // get input if not the first command
             if (i > 0) {
                 if (dup2(pipefds[(i - 1) * 2], STDIN_FILENO) == -1) {
                     perror("dup2 input");
                     exit(ERR_EXEC_CMD);
                 }
             }
 
             if (i < num_cmds - 1) {
                 if (dup2 (pipefds[i * 2 + 1], STDOUT_FILENO) == -1) {
                     perror("dup2 output"); 
                     exit(ERR_EXEC_CMD);
                 }
             }
 
             //Last command output
             if (clist->commands[i].output_file != NULL) {
                 int fd;
                 if (clist->commands[i].append_mode) {
                     fd = open(clist->commands[i].output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
                 } else { 
                     fd = open(clist->commands[i].output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                 }
 
                 if (fd == -1) {
                     perror("Error opening file for redirection"); 
                     exit(ERR_EXEC_CMD);
                 }
 
                 if (dup2(fd, STDOUT_FILENO) == -1){
                     perror("Error redirecting output to file");
                     exit(ERR_EXEC_CMD);
                 }
                 close(fd); 
             }
 
             for (int x = 0; x < 2* (num_cmds - 1); x++) { 
                 close (pipefds[x]);
             }        
 
             //execute cmd
             execvp(clist->commands[i].argv[0], clist->commands[i].argv);
             perror("execvp");
             exit(ERR_EXEC_CMD);    
         }
     }
 
     // Close all pipes in the parent
     for (int i = 0; i < 2 * (num_cmds - 1); i++) {
         close(pipefds[i]);
     }
 
     // Wait for all children to finish
     for (int i = 0; i < num_cmds; i++) {
         wait(NULL);
     }
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
      cmd_buff->output_file = NULL;
      cmd_buff->append_mode = false;
      bool quotes=false;
      
      while (*ptr){
          while (isspace((unsigned char)*ptr) && !quotes){
              ptr++;
          } // skipping spaces
  
          // redirection handling
          if(*ptr == '\0'){
            break;  
          }
 
 
          // Redirection
          if (*ptr == '>'){
             ptr++;  // Move past the >
             while (isspace((unsigned char)*ptr)) ptr++; // Skip all the spaces after '>'
             if (*ptr == '>') {
                 cmd_buff->append_mode = true;
                 ptr++; // Move past the >
             }
             
             cmd_buff->output_file = ptr; //set output
             while (*ptr && !isspace((unsigned char)*ptr)) ptr++;
             if (*ptr) *ptr++ = '\0'; // Null-terminate the output filepath
             continue;
         }
  
         // regular argument processing
         cmd_buff->argv[cmd_buff->argc++] = ptr;
         while (*ptr && !isspace((unsigned char)*ptr)) {
             ptr++;
         }
  
          if(*ptr) *ptr++ = '\0'; // Null-terminate arg  
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
          return WARN_NO_CMDS;
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
          default:
              return BI_NOT_BI;
          }
  }
  
  
  
 int build_cmd_list(char *cmd_line, command_list_t *clist){
     if (!cmd_line || !clist) return ERR_MEMORY;
 
     clist->num = 0;
     char *cmd_copy = strdup(cmd_line); // copy input cmd line
 
     if (!cmd_copy) return ERR_MEMORY;
     
     char *token = strtok(cmd_copy, PIPE_STRING);
 
     while (token != NULL) {
         if (clist->num >= CMD_MAX){
             free(cmd_copy);
             return ERR_TOO_MANY_COMMANDS;
         }
 
         if(build_cmd_buff(token, &clist->commands[clist->num]) != OK) return ERR_EXEC_CMD;
 
         clist->num++;
         token = strtok(NULL, PIPE_STRING); // Next command
     }
 
     free(cmd_copy);
     return (clist->num > 0) ? OK : WARN_NO_CMDS;
 }
 
 
 
 
 
 
  
  int exec_cmd(cmd_buff_t *cmd) {
     pid_t pid = fork();
     if (pid == 0) { // Child process
         if (cmd->output_file != NULL) { // Handle output redirection
             int fd = open(cmd->output_file, O_WRONLY | O_CREAT | (cmd->append_mode ? O_APPEND : O_TRUNC), 0644);
             if (fd == -1) {
                 perror("Error opening file for redirection");
                 exit(ERR_EXEC_CMD);
             }
             if (dup2(fd, STDOUT_FILENO) == -1) {
                 perror("Error redirecting output to file");
                 exit(ERR_EXEC_CMD);
             }
             close(fd);
         }
 
         execvp(cmd->argv[0], cmd->argv);
         perror("execvp");
         exit(ERR_EXEC_CMD);
     } else if (pid > 0) { // Parent process
         int status;
         waitpid(pid, &status, 0);
         if (WEXITSTATUS(status)) {
             int exit_code = WEXITSTATUS(status);
             if (exit_code != 0) { // Something went wrong
                 errno = exit_code;
                 return ERR_EXEC_CMD;
             }
         }
         return OK;
     } else {
         perror("fork");
         return ERR_EXEC_CMD;
     }
 }
 

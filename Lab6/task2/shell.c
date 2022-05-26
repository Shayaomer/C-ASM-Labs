#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "LineParser.h"
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define STD_IN 0
#define STD_OUT 1
#define STD_ERR 2


void execute(cmdLine* to_exe, int debug_flag, int* fd){
    int status;
    int pid = fork();
    if(debug_flag == 1){
        fprintf(stderr, "PID: %d, Excecution command: %s\n", pid, to_exe->arguments[0]);
    }
    if(pid == 0){
        /* Child process */
        if (to_exe->inputRedirect) {
            close(STD_IN); // closing default stdin
            open(to_exe->inputRedirect, O_RDONLY, 0700);
        }
        if (to_exe->outputRedirect) {
            close(STD_OUT); // closing default stdin
            open(to_exe->outputRedirect, O_WRONLY | O_CREAT, 0700);
        }

        if (to_exe->next){ //if it has another command we will change his output to the next command
            dup2(fd[1], STD_OUT);
        }
        if (to_exe->idx > 0){ //if its not the first command, the input will be from prev command
            dup2(fd[0], STD_IN);
        }
        /* Executing: */
        int res = execvp(to_exe->arguments[0], to_exe->arguments);
        if(res == -1){
            perror("Execute failed");
            _exit(1);
        } 
    } else {
        /*Parent process */
        if (to_exe ->blocking == 1)
            waitpid(pid, &status, 0);
        wait(NULL);
        close(fd[1]);
    }
     
    
}

int main(int argc, char **argv) {
    char _path[PATH_MAX];
    char user_input[2048];
    int debug_flag = 0;

    /*Parsing args: */
    int i;
    for (i=1; i < argc; i++){
        if(strcmp(argv[i], "-d") == 0){
            debug_flag = 1;
        }
    }
    while (1){
        if(getcwd(_path, sizeof(_path)) == NULL){
            perror("Error ");
        } else{
            int fd[2];
            pipe(fd); //create a new pipe for every new excecution 

            printf("%s$ ", _path); 
            fgets(user_input, sizeof(user_input), stdin);
            cmdLine* parsed_input = parseCmdLines(user_input);
            cmdLine* cur_input = parsed_input;
            while(cur_input){
                if(strcmp(cur_input->arguments[0], "quit") == 0){
                _exit(1);
                }
                if(strcmp(cur_input->arguments[0], "cd") == 0){
                    /* CD function */
                        int chdir_res = chdir(cur_input->arguments[1]);
                        if (chdir_res == -1){
                            fprintf(stderr, "Changing dir failed.\n");
                        }
                } else{
                    execute(cur_input, debug_flag, fd); 
                }   
                cur_input = cur_input->next; 
            }
               
        }
            
    }
}
        
        

    

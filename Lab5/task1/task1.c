#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "LineParser.h"
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>


void execute(cmdLine* to_exe, int debug_flag){
    int pid = fork();
    int status;
    if(debug_flag == 1){
        fprintf(stderr, "PID: %d, Excecution command: %s\n", pid, to_exe->arguments[0]);
    }
    if(pid == 0){
        /* Child process */
        int res = execvp(to_exe->arguments[0], to_exe->arguments);
        if(res == -1){
            perror("Execute failed");
            _exit(1);
        } 
        
    } else {
        /*Parent process */
        if (to_exe ->blocking == 1)
            waitpid(pid, &status, 0);
    }
    freeCmdLines(to_exe); 
}

int main(int argc, char **argv) {
    char _path[PATH_MAX];
    char user_input[2048];
    cmdLine* parsed_input;
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
            perror("Error : can't get current directory");
        } else{
            printf("%s$ ", _path); 
            fgets(user_input, sizeof(user_input), stdin);
            parsed_input = parseCmdLines(user_input);
            
            if(strcmp(parsed_input->arguments[0], "quit") == 0){
                _exit(1);
            }

            if(strcmp(parsed_input->arguments[0], "cd") == 0){
                /* CD function */
                    int chdir_res = chdir(parsed_input->arguments[1]);
                    if (chdir_res == -1){
                        fprintf(stderr, "Changing dir failed.\n");
                    }
            } else{
                execute(parsed_input, debug_flag); 
            }       
        }
            
    }
}
        
        

    

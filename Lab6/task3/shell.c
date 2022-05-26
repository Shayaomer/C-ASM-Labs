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

void close_all_pipes(int ** pipes, int pipes_size){
    for (int i = 0; i < pipes_size; i++){
        close(pipes[i][1]);
    }
}

int ll_length(cmdLine* _line){
    int count = 0;
    cmdLine* temp = _line;
    while(temp){
        count++;
        temp = temp->next;
    }
    return count;
}

int ** createPipes(int nPipes){
    int** pipes;
    pipes=(int**) calloc(nPipes, sizeof(int));

    for (int i=0; i<nPipes;i++){
        pipes[i]=(int*) calloc(2, sizeof(int));
        pipe(pipes[i]);
    }  
    return pipes;
}

void releasePipes(int **pipes, int nPipes){
        for (int i=0; i<nPipes;i++){
            free(pipes[i]);
        }  
    free(pipes);
}

int *leftPipe(int **pipes, cmdLine *pCmdLine){
    if (pCmdLine->idx == 0) return NULL;
    return pipes[pCmdLine->idx -1];
}

int *rightPipe(int **pipes, cmdLine *pCmdLine){
    if (pCmdLine->next == NULL) return NULL;
    return pipes[pCmdLine->idx];
}

void execute(cmdLine* to_exe, int debug_flag, int** pipes, int pipe_size){
    int status;
    int* left_pipe = leftPipe(pipes, to_exe);
    int* right_pipe = rightPipe(pipes, to_exe);
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
            dup2(right_pipe[1], STD_OUT);
        }
        if (to_exe->idx > 0){ //if its not the first command, the input will be from prev command
            dup2(left_pipe[0], STD_IN);
        }
        /* Executing: */
        close_all_pipes(pipes, pipe_size -1);
        execvp(to_exe->arguments[0], to_exe->arguments);
        exit(1);
    } else {
        /*Parent process */
        waitpid(pid, &status, 0); 
        if(to_exe->next)
            close(pipes[to_exe->idx][1]);
    }
     
    
}

int main(int argc, char **argv) {
    char _path[PATH_MAX];
    char user_input[2048];
    int debug_flag = 0;
    int **pipes;
    int pipes_size;

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
            printf("%s$ ", _path);  // print the shell line
            fgets(user_input, sizeof(user_input), stdin);
            cmdLine* parsed_input = parseCmdLines(user_input);

            if(parsed_input->idx == 0){ // get the linked list size and create pipe array
                pipes_size = ll_length(parsed_input);
                pipes = createPipes(pipes_size -1);
            }

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
                    execute(cur_input, debug_flag, pipes, pipes_size); 
                }   
                cur_input = cur_input->next; 
            }
               
        }
            
    }
}
        
        

    

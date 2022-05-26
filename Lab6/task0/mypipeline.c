#include "LineParser.h"
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void debug_print(char* to_print, int flag){
    if(flag){
        fprintf(stderr, "%s\n", to_print);
    }
}


int main(int argc, char *argv[]){
    int fd[2];
    int d_flag = 0;
    int status;

    /* Parsing args */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            d_flag = 1;
        }
    }

    /* Creating pipe */
    if(pipe(fd) == -1){
        printf("pipe failed");
        exit(1);
    }
    debug_print("(parent_process>forking...)", d_flag);
    /* FORK #1 */
    int pid = fork();
    if (pid == 0){
        /* Child1 process */
        debug_print("(child1>redirecting stdout to the write end of the pipe...)", d_flag);
        if (fclose(stdout) > 0){
            perror("closing stdout failed");
            _exit(1);
        }
        dup(fd[1]);
        close (fd[1]);
        debug_print("(child1>going to execute cmd: ...)", d_flag);
        char* ls_arg[] = {"ls", "-l", NULL};
        if ((execvp("ls", ls_arg) == -1)){
            perror("execvp failed");
            _exit(1);
        }

    } else{
        /* Parent process */
        if(d_flag){
            fprintf(stderr, "(parent_process>created process with id: %d)\n", pid);
        }
        debug_print("(parent_process>closing the write end of the pipe...)", d_flag);
        if (close(fd[1]) > 0){
            perror("closing stdout failed");
            _exit(1);
        }
        /* FORK #2 */
        int pid2 = fork();
        if(pid2 == 0){
            /* Child2 process */
            debug_print("(child2>redirecting stdin to the read end of the pipe...)", d_flag);
            if (fclose(stdin) > 0){
            perror("closing stdin failed");
            _exit(1);
            }
            dup(fd[0]);
            close (fd[0]);
            debug_print("(child2>going to execute cmd: ...)", d_flag);
            char* tail_arg[] = {"tail", "-n", "2", NULL};
            if ((execvp("tail", tail_arg) == -1)){
                perror("execvp2 failed");
                _exit(1);
            }
        } else {
            /* Parent process */
            debug_print("(parent_process>closing the read end of the pipe...)", d_flag);
            close(fd[0]);
            debug_print("(parent_process>waiting for child processes to terminate...)", d_flag);
            waitpid(pid, &status, 0);
            waitpid(pid2, &status, 0);
        }
    }
}

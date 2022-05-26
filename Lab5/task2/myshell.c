#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "LineParser.h"
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define TERMINATED -1
#define RUNNING 1
#define SUSPENDED 0
#define SEND_NOTHING 2
#define MAX_USER_INPUT 2048

typedef struct process{
    cmdLine* cmd;   /* the parsed command line*/
    pid_t pid;      /* the process id that is running the command*/
    int status;     /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next;   /* next process in chain */
} process;

void addProcess(process** process_list, cmdLine* cmd, pid_t pid);
void printProcessList(process** process_list);
void freeProcessList(process* process_list);
void updateProcessList(process **process_list);
void updateProcessStatus(process* process_list, int pid, int status);
void execute(cmdLine* to_exe, int debug_flag);


void addProcess(process** process_list, cmdLine* cmd, pid_t pid){
    process* new_proc = malloc(sizeof(process));
    new_proc->cmd = cmd;
    new_proc->pid = pid;
    new_proc->status = RUNNING;
    if ((*process_list) == NULL){
        new_proc->next = NULL;
        *process_list = new_proc;
    } else{
        new_proc->next = *process_list;
        *process_list = new_proc;
    }
}

void printProcessList(process** process_list){
    updateProcessList(process_list);
    process* temp_proc = *process_list;
    printf("%s     %s     %s\n","PID", "Command", "STATUS");
    while(temp_proc != NULL){
        printf("%ld     %s     %d", (long)temp_proc->pid,
                                    temp_proc->cmd->arguments[0],
                                    temp_proc->status);
        temp_proc = temp_proc->next;
    }
}

void freeProcessList(process* process_list){
    if(process_list->next != NULL){
        freeProcessList(process_list->next);
    }
    free(process_list);
}

void updateProcessList(process **process_list){
    process *p = *process_list;
    int status = 0;
    int return_code = 0;
    int status_to_send = SEND_NOTHING;
    while (p != NULL) {
        return_code = waitpid(p->pid, &status, WNOHANG | WUNTRACED | WCONTINUED);
        if (return_code == -1) {
            printf("Error in the process");
            status_to_send = TERMINATED;
        } else if (return_code == 0) {
            // nothing has changes child in previous state still.
        } else if (WIFEXITED(status)) {
            // Child exited.
            status_to_send = TERMINATED;
        } else if (WIFSIGNALED(status)) {
            // Child killed by a signal.
            status_to_send = TERMINATED;
        } else if (WIFSTOPPED(status)) {
            // Child stopped.
            status_to_send = SUSPENDED;
        } else if (WIFCONTINUED(status)) {
            // children continue after suspended
            status = RUNNING;
        }
        if(status != SEND_NOTHING){
            updateProcessStatus(*process_list, p->pid, status_to_send);
        }
        status_to_send = SEND_NOTHING;
        p = p->next;
    }
}

void updateProcessStatus(process* process_list, int pid, int status){
    process *p = process_list;
    if(p == NULL){
        return;
    }
    if(p->pid != pid){
        updateProcessStatus(p->next, pid, status);
        return;
    }
    p->status = status;
}

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

/* Main function: */
int main(int argc, char **argv) {
    char _path[PATH_MAX];
    char user_input[2048];
    cmdLine* parsed_input;
    int debug_flag = 0;
    process* process_list = NULL;

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
                /* Quit function */
                _exit(1);
            } else if (strcmp(parsed_input->arguments[0], "showprocs") == 0) {
                /* showprocs function */
                printProcessList(&process_list);
            } else if(strcmp(parsed_input->arguments[0], "cd") == 0){
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
        
        

    

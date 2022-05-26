#include "util.h"

#define SYS_EXIT 1
#define SYS_WRITE 4
#define SYS_READ 3
#define STD_ERR 2
#define STD_OUT 1
#define STD_IN 0
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define O_RDWR 2
#define O_RDONLY 0
#define O_WRONLY 1


int system_call();

/* Support functions: */

void exit_func(char* error){
    system_call(SYS_WRITE, STD_ERR, error, strlen(error));
    system_call(SYS_EXIT);
}

void new_line(int stream){
    system_call(SYS_WRITE, stream, "\n", 1);
}

void print_sys_call(int op_code, int res_arg){
    system_call(SYS_WRITE, STD_ERR, "system call [ID, ret code] = ", strlen("system call [ID, ret code] = "));
    system_call(SYS_WRITE, STD_ERR, itoa(op_code), 1);
    system_call(SYS_WRITE, STD_ERR, ", ", strlen(", "));
    system_call(SYS_WRITE, STD_ERR, itoa(res_arg), 1);
    new_line(STD_ERR);
}

int system_call_write(int stream, char* buf, int str_size, int debug_flag){
    int res = system_call(SYS_WRITE, stream, buf, str_size);
    new_line(stream);
    if(debug_flag == 1){
        print_sys_call(SYS_WRITE, res);
    }
    return res;
}

int system_call_read(int stream, char* buf, int str_size, int debug_flag){
    int res = system_call(SYS_READ, stream, buf, str_size);
    if(debug_flag != 0){
        print_sys_call(SYS_READ, res);
    }
    return res;
}

int system_call_open(char* path_name, int mode, int perms, int debug_flag){
    int res = system_call(SYS_OPEN, path_name, mode, perms);
    if (debug_flag == 1){
        print_sys_call(SYS_OPEN, res);
    }
    return res;
}

int system_call_close(int fd, int debug_flag){
    int res = system_call(SYS_CLOSE, fd);
    if (debug_flag == 1){
        print_sys_call(SYS_CLOSE, res);
    }
    return res;
}


/* Main function: */

int main (int argc , char* argv[], char* envp[]) {
    
    /* Flags: */ 
    int debug_flag = 0;
    int file_input_flag = 0;
    int file_output_flag = 0;

    /* Other support vars: */
    char* file_in_name = "";
    char* file_out_name = "";
    char curr_char;
    int byte_read = 0;
    int fin_read = 0;
    int words_counter = 0;
    int in_space = 1;
    char* error_input = "Error opening input file, quitting...\n";
    char* error_output = "Error opening output file, quitting...\n";

    int input_fd = STD_IN;
    int output_fd = STD_OUT;
    
    /* Args parse: */
    int i;
    for (i=1; i < argc; i++){
        if(strcmp(argv[i], "-D") == 0){
            debug_flag = 1;
        }

        if(strncmp(argv[i], "-i", 2) == 0){
            file_input_flag = 1;
            file_in_name = argv[i] + 2;
            if (debug_flag == 1){
                system_call_write(STD_ERR, file_in_name, strlen(file_in_name), debug_flag);
            }
            input_fd = system_call_open(file_in_name, O_RDONLY, 0777, debug_flag);
            if(input_fd < 0){
                exit_func(error_input);
            }
        }

        if(strncmp(argv[i], "-o", 2) == 0){
            file_output_flag = 1;
            file_out_name = argv[i] + 2;
            if (debug_flag == 1){
                system_call_write(STD_ERR, file_out_name, strlen(file_out_name), debug_flag);
            }
            output_fd = system_call_open(file_out_name, O_WRONLY, 0777, debug_flag);
            if (output_fd < 0){
                exit_func(error_output);
            }
        }  
    }

    /* stdin/stdout debug outputs: */
    if(debug_flag == 1 && file_input_flag == 0){
        system_call_write(STD_ERR, "stdin", strlen("stdin"), debug_flag);
    }
    if (debug_flag == 1 && file_output_flag == 0) {
            system_call_write(STD_ERR, "stdout", strlen("stdout"), debug_flag);
    }
        
    /* Counter code: */
    while (fin_read == 0){
        byte_read = system_call_read(input_fd, &curr_char, 1, debug_flag);
        if (byte_read > 0){
            if (curr_char == 10){
                system_call_write(output_fd, itoa(words_counter), strlen(itoa(words_counter)), debug_flag);
                words_counter = 0;
                in_space = 1;
            } else if (curr_char == ' '){
                in_space = 1;
            } else {
                words_counter += in_space;
                in_space = 0;
            }
        } else{
            if (file_input_flag == 1){
                system_call_close(input_fd, debug_flag);
            }
            if (file_output_flag == 1){
                system_call_close(output_fd, debug_flag);
            }
            fin_read = 1;
        }
    }
    
    return 0;

}
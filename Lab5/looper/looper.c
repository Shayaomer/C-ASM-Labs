#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define SIGINT 2
#define SIGTSTP 20
#define SIGCONT 18


void handler(int signum){
	char* sig_name;
	sig_name = strsignal(signum);
	printf("The signal caught is: %s\n", sig_name);
	signal(signum, SIG_DFL);
	raise(signum);
}

int main(int argc, char **argv){ 
	signal(SIGINT, handler);
	/*signal(SIGTSTP, handler);
	signal(SIGCONT, handler);*/
	printf("Starting the program\n");

	while(1) {
		sleep(2);
	}

	return 0;
}


#include "util.h"

#define SYS_WRITE 4
#define STDOUT 1

int system_call();

int main (int argc , char* argv[], char* envp[])
{
  int i;
  char * str;
  str = itoa(argc);
  system_call(SYS_WRITE,STD_OUT, str,strlen(str));
  system_call(SYS_WRITE,STD_OUT," arguments \n", 12);
  for (i = 0 ; i < argc ; i++)
    {
      system_call(SYS_WRITE,STD_OUT,"argv[", 5);
	  str = itoa(i);
      system_call(SYS_WRITE,STD_OUT,str,strlen(str));
      system_call(SYS_WRITE,STD_OUT,"] = ",4);
      system_call(SYS_WRITE,STD_OUT,argv[i],strlen(argv[i]));
      system_call(SYS_WRITE,STD_OUT,"\n",1);
    }
  return 0;
}


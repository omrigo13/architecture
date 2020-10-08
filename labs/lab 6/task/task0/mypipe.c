#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LineParser.h"
#include "linux/limits.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define TERMINATED  -1
#define RUNNING 1
#define SUSPENDED 0

int debugMode = 0;

typedef struct process{
    cmdLine* cmd;                         /* the parsed command line*/
    pid_t pid; 		                  /* the process id that is running the command*/
    int status;                           /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next;	                  /* next process in chain */
} process;

int main (int argc , char* argv[], char* envp[]) 
{
    char* in = malloc(2048);
    int pid = 0;
    int pipefd[2];

    for (int i = 0;i < argc; i++)
    if (strcmp(argv[i],"-d") == 0)
        debugMode = 1;

    pipe(pipefd);
    
    if ((pid = fork()) == 0)
    {
        close(pipefd[0]);
        write(pipefd[1], "hello\n", 5);
        exit(0);
    }

    close(pipefd[1]);
    read(pipefd[0], in, 2048);
    printf("%s\n", in);
}
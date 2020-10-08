#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LineParser.h"
#include "linux/limits.h"
#include <unistd.h>
#include <sys/types.h>

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

void addProcess(process** process_list, cmdLine* cmd, pid_t pid)
{
  /* Receive a process list (process_list), a command (cmd), and the process id (pid) of the process running the command */
  process* proc = malloc(sizeof(process));
  proc->cmd = cmd;
  proc->pid = pid;
  proc->status = RUNNING;
  proc->next = *process_list;
  *process_list = proc;
}

void printProcessList(process** process_list)
{    
  process* procList = *process_list;
  
  printf("index\t\tPID\t\tCommand\t\tSTATUS\n");
  int index = 1;
  while(procList != NULL)
  {
    printf("%d\t\t%d\t\t%s\t\t", index, procList->pid, procList->cmd->arguments[0]);
    switch (procList->status)
    {
      case RUNNING:
        printf("Running\n");
        break;
      case TERMINATED:
        printf("Terminated\n");
        break;
      case SUSPENDED:
        printf("Suspended\n");
        break;  
      default:
        break;
    }
    index++;
    procList = procList->next;
  }
}

void debug(int pid, cmdLine* cmd)
{
  if (debugMode)
    fprintf(stderr, "PID: %d, Executing command: %s\n", pid, cmd->arguments[0]);
}

int execute(cmdLine *pCmdLine)
{
  pid_t cpid = 0;
  cpid = fork();

  if(cpid == -1)
  {
    perror("fork failed");
    exit(-1);
  }
  if(cpid == 0)
  {
    if(execvp(pCmdLine->arguments[0], pCmdLine->arguments) == -1) 
    {
      perror("error in execute");
      exit(-1);
    }
  }
  debug(cpid, pCmdLine);
  if (pCmdLine->blocking)
    waitpid(cpid, NULL, 0);
    
  return cpid;
}

int command(cmdLine *pCmdLine, process** process_list)
{
  int ret = 0;
  if (strcmp(pCmdLine->arguments[0],"quit") == 0)
  {
    freeCmdLines(pCmdLine);
    exit(0);
  }
  else if (strcmp(pCmdLine->arguments[0],"cd") == 0)
  {
    ret = 1;
    if (chdir(pCmdLine->arguments[1]))
      perror("failed to do cd operation");
    freeCmdLines(pCmdLine);
  }
  else if (strcmp(pCmdLine->arguments[0],"procs") == 0)
  {
    ret = 1;
    printProcessList(process_list);
    freeCmdLines(pCmdLine);
  }

  return ret;
}

int main(int argc , char* argv[], char* envp[]) 
{
  char* path = malloc(PATH_MAX);
  char* in = malloc(2048);
  cmdLine* parseCmdLine;
  int pid = 0;
  process* process_list = NULL;

  for (int i = 0;i < argc; i++)
    if (strcmp(argv[i],"-d") == 0)
        debugMode = 1;

  while(1)
  {
    getcwd(path, PATH_MAX);
    printf("%s>", path);
    fgets(in, 2048, stdin);
    parseCmdLine = parseCmdLines(in);
    if (parseCmdLine == NULL)
        continue;
    if (command(parseCmdLine, &process_list))
        continue;
    pid = execute(parseCmdLine);
    addProcess(&process_list, parseCmdLine, pid);
  }
}
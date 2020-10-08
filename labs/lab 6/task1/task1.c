#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LineParser.h"
#include "linux/limits.h"
#include <unistd.h>
#include <sys/types.h>

int debugMode = 0;

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
    /* lab 6 new code */
    if(pCmdLine->inputRedirect)
    {
      fclose(stdin);
      fopen(pCmdLine->inputRedirect,"r");
    }

    if (pCmdLine->outputRedirect)
    {
      fclose(stdout);
      fopen(pCmdLine->outputRedirect,"w");
    }
    /* end of lab 6 new code */
    
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

int command(cmdLine *pCmdLine)
{
  int ret = 0;
  if (strcmp(pCmdLine->arguments[0],"quit") == 0)
  {
    freeCmdLines(pCmdLine);
    exit(0);
  }
  if (strcmp(pCmdLine->arguments[0],"cd") == 0)
  {
    ret = 1;
    if (chdir(pCmdLine->arguments[1]))
      perror("failed to do cd operation");
    freeCmdLines(pCmdLine);
  }

  return ret;
}

int main(int argc , char* argv[], char* envp[]) 
{
  char* path = malloc(PATH_MAX);
  char* in = malloc(2048);
  cmdLine* parseCmdLine;

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
    if (command(parseCmdLine))
        continue;
    execute(parseCmdLine);
    freeCmdLines(parseCmdLine);
  }
}
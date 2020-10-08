#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LineParser.h"
#include "linux/limits.h"
#include <unistd.h>

void execute(cmdLine *pCmdLine)
{
  int err = execvp(pCmdLine->arguments[0], pCmdLine->arguments);
  if (err == -1) 
  {
    perror("error in execute function on task0");
    exit(-1);
  }
  return;
}

int main (int argc , char* argv[], char* envp[]) 
{
  char* path = malloc(PATH_MAX);
  char* in = malloc(2048);
  cmdLine* parseCmdLine;

  while(1)
  {
    getcwd(path, PATH_MAX);
    printf("%s>", path);
    fgets(in, 2048, stdin);
    parseCmdLine = parseCmdLines(in);
    if (strcmp(parseCmdLine->arguments[0], "quit") == 0)
        exit(0);
    execute(parseCmdLine);
    freeCmdLines(parseCmdLine);
  }
}
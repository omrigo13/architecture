#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LineParser.h"
#include "linux/limits.h"
#include <unistd.h>
#include <sys/types.h>

typedef struct variable
{
  char* name;             /* variable name */
  char* value;            /* variable value */
  struct variable *next;  /* next variable in linked list */
} variable;

int debugMode = 0;

void debug(int pid, cmdLine* cmd)
{
  if (debugMode)
    fprintf(stderr, "PID: %d, Executing command: %s\n", pid, cmd->arguments[0]);
}

void setVar(variable** var_list, cmdLine* cmd)
{
  variable* var = *var_list;
  while(var != NULL && (strcmp(var->name, cmd->arguments[1]) != 0))
    var = var->next;
  
  if (var != NULL)
  {
    free(var->value);
    var->value = strdup(cmd->arguments[2]);
  }
  else
  {
    variable* newVar = malloc(sizeof(variable));
    newVar->name = strdup(cmd->arguments[1]);
    newVar->value =  strdup(cmd->arguments[2]);
    newVar->next = *var_list;
    *var_list = newVar;
  }  
}

void printVarList(variable* var_list)
{
  while(var_list != NULL)
  {
    printf("variable name: %s, variable value: %s\n", var_list->name, var_list->value);
    var_list = var_list->next;
  }
}

int replaceVar(cmdLine* cmd, variable* newVar){

    int ret = 0;

    for(int i = 0; i < cmd->argCount; i++)
    {
      if (cmd->arguments[i][0] == '$' || cmd->arguments[i][0] == '~')
      {
        while(newVar != NULL && ((strcmp(newVar->name, (cmd->arguments[i]+1)) != 0) && (strcmp(newVar->name, cmd->arguments[i]) != 0)))
          newVar = newVar->next;
        if (newVar != NULL)
          replaceCmdArg(cmd, i, newVar->value);
        else
        {
          ret = -1;
          printf("no variable: %s\n", cmd->arguments[i]+1);
        }
      }
    }

  return ret;
}

void freeVars(variable* var_list)
{
  while(var_list != NULL)
  {
    variable* freeVar = var_list;
    var_list = var_list->next;
    free(freeVar->name);
    free(freeVar->value);
    free(freeVar);
  }
}

int execute(cmdLine *pCmdLine)
{
  pid_t cpid = 0;
  int pipefd1[2], pipefd2[2];

  if(pCmdLine->next)
    pipe(pipefd1);

  pipefd2[0] = pipefd1[0];
  pipefd2[1] = pipefd1[1];

  cpid = fork();

  if(cpid == -1)
  {
    perror("fork failed");
    exit(-1);
  }
  if(cpid == 0)
  {

    if(pCmdLine->next)
      dup2(pipefd1[1], fileno(stdout));  

    /* lab 6 new code */
    if(pCmdLine->inputRedirect)
    {
      fclose(stdin);
      fopen(pCmdLine->inputRedirect,"r");
    }

    if(pCmdLine->outputRedirect)
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

  if(pCmdLine->next)
    close(pipefd1[1]);

   while(pCmdLine->next)
   {
      pCmdLine = pCmdLine->next;
      pipefd1[0] = pipefd2[0];
      pipefd1[1] = pipefd2[1];
        
      if(pCmdLine->next)
        pipe(pipefd2);
        
      cpid = fork();

      if(cpid == -1)
      {
        perror("fork failed");
        exit(-1);
      }

      if(cpid == 0)
      {
        dup2(pipefd1[0], fileno(stdin));
        if(pCmdLine->next)
          dup2(pipefd2[1], fileno(stdout));
            
        /* lab 6 new code */
        if(pCmdLine->inputRedirect)
        {
          fclose(stdin);
          fopen(pCmdLine->inputRedirect,"r");
        }

        if(pCmdLine->outputRedirect)
        {
          fclose(stdout);
          fopen(pCmdLine->outputRedirect,"w");
        }
        /* end of lab 6 new code */
        
        if(execvp(pCmdLine->arguments[0], pCmdLine->arguments) == -1)
        {
          perror("error in execution");
          exit(-1);
        }
      }
      debug(cpid, pCmdLine);
      close(pipefd1[0]);
      if (pCmdLine->next)
        close(pipefd2[1]);
    }
    
  if (pCmdLine->blocking)
    waitpid(cpid, NULL, 0);
    
  return cpid;
}

int command(cmdLine *pCmdLine, variable** var_list)
{
  int ret = 0;
  if(strcmp(pCmdLine->arguments[0],"quit") == 0)
  {
    freeCmdLines(pCmdLine);
    exit(0);
  }
  else if(strcmp(pCmdLine->arguments[0],"cd") == 0)
  {
    ret = 1;
    if (chdir(pCmdLine->arguments[1]))
      perror("failed to do cd operation");
    freeCmdLines(pCmdLine);
  }
  else if(strcmp(pCmdLine->arguments[0],"set") == 0)
  {      
    ret = 1;
    setVar(var_list, pCmdLine);      
    freeCmdLines(pCmdLine);
  }
  else if(strcmp(pCmdLine->arguments[0],"vars") == 0)
  {
    ret = 1;
    printVarList(*var_list);      
    freeCmdLines(pCmdLine);
  }

  return ret;
}

int main(int argc , char* argv[], char* envp[]) 
{
  char* path = malloc(PATH_MAX);
  char* in = malloc(2048);
  cmdLine* parseCmdLine;
  int varExist = 0;
  variable* var_list = malloc(sizeof(variable));

  var_list->name = strdup("~");
  var_list->value = strdup(getenv("HOME"));
  var_list->next = NULL;

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
    cmdLine* replaceCmd = parseCmdLine;
    while (replaceCmd != NULL)
    {
      if((varExist = replaceVar(replaceCmd, var_list)))
        break;      
      replaceCmd = replaceCmd->next;        
    }
    if (command(parseCmdLine, &var_list))
        continue;
    execute(parseCmdLine);
    freeCmdLines(parseCmdLine);
  }
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int debugMode = 0;

int main(int argc , char* argv[], char* envp[]) 
{
	for (int i = 0;i < argc; i++)
    	if (strcmp(argv[i],"-d") == 0)
        	debugMode = 1;

	int pipefd[2];
	pid_t child1, child2;

  	pipe(pipefd);
  	child1 = fork();
	if(debugMode)
		fprintf(stderr, "(parent_process>created process with id: %d)\n", child1);

  	if(child1 == -1)
  	{
    	perror("error in fork child1");
    	exit(-1);
 	}
  	//child1
  	else if(child1 == 0) 
  	{
		if(debugMode)
			fprintf(stderr, "(child1>redirecting stdout to the write end of the pipe…)\n");
		fclose(stdout);
		dup(pipefd[1]); //dup the write-end of the pipe
		close(pipefd[1]);
		char* cmd1[3];
		cmd1[0]= "ls";
		cmd1[1]= "-l";
		cmd1[2]=0;
		if(debugMode)
			fprintf(stderr, "(child1>going to execute cmd: %s)\n", cmd1[0]);
		if(execvp(cmd1[0], cmd1) == -1) 
    	{
			perror("error in execute of child1");
			exit(-1);
		}
	}
  	//parent
  	else
  	{
		if(debugMode)
			fprintf(stderr, "(parent_process>closing the write end of the pipe…)\n");
		close(pipefd[1]); //close the write end of the pipe
		if(debugMode)
			fprintf(stderr, "(parent_process>forking…)\n");
		child2 = fork();
		if(debugMode)
			fprintf(stderr, "(parent_process>created process with id: %d)\n", child2);

		if(child2 == -1) 
    	{
		  	perror("error in fork child2");
		  	exit(-1);
		}
    	//child2
    	else if(child2 == 0)
    	{
			if(debugMode)
				fprintf(stderr, "(child2>redirecting stdin to the read end of the pipe…)\n");
			fclose(stdin);
			dup(pipefd[0]); //dup the read-end of the pipe
			close(pipefd[0]);
			char* cmd2[4];
			cmd2[0]= "tail";
			cmd2[1]= "-n";
			cmd2[2]="2";
			cmd2[3]=0;
			if(debugMode)
				fprintf(stderr, "(child2>going to execute cmd: %s)\n", cmd2[0]);
			if(execvp(cmd2[0], cmd2) == -1) 
      		{
				perror("error in execute of child2");
				exit(-1);
			}
    	}
		else
		{
			if(debugMode)
				fprintf(stderr, "(parent_process>closing the read end of the pipe…)\n");
			close(pipefd[0]); //close the read end of the pipe
			int status;
			if(debugMode)
				fprintf(stderr, "(parent_process>waiting for child1 processes to terminate…)\n");
			waitpid(child1,&status,0);
			if(debugMode)
				fprintf(stderr, "(parent_process>waiting for child2 processes to terminate…)\n");
			waitpid(child2,&status,0);
			if(debugMode)
				fprintf(stderr, "(parent_process>exiting…)\n");
			exit(0);
		}
  	}
}
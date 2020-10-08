#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void sig_handler(int signo)
{
	printf("received %s\n", strsignal(signo));
	if (signo == SIGCONT)
		signal(SIGTSTP, sig_handler);
	
	if (signo == SIGTSTP)
		signal(SIGCONT, sig_handler);
	
	signal(signo, SIG_DFL);
	raise(signo);
	return;
}

int main(int argc, char **argv){ 

	printf("Starting the program\n");

	if (signal(SIGTSTP, sig_handler) == SIG_ERR)
		printf("\ncan't catch SIGTSTP\n");

	if (signal(SIGINT, sig_handler) == SIG_ERR)
		printf("\ncan't catch SIGINT\n");

	if (signal(SIGCONT, sig_handler) == SIG_ERR)
		printf("\ncan't catch SIGCONT\n");

	while(1) {
		sleep(2);
	}

	return 0;
}
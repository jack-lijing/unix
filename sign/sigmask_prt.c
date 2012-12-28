#include "csapp.h"

void sig_quit(int signo)
{
	printf("caught SIGQUIT\n");

	Signal(SIGQUIT, SIG_DFL);
	return;
}

int main(int argc, char *argv[])
{
	sigset_t		newmask, oldmask, pendmask;

	Signal(SIGQUIT, sig_quit);

	Sigemptyset(&newmask);
	Sigaddset(&newmask, SIGQUIT);
	Sigprocmask(SIG_BLOCK, &newmask, &oldmask);
	pr_mask("Main:");
	Sleep(2);			/*SIGQUIT here will remain pending*/

	sigpending(&pendmask);

	if(sigismember(&pendmask, SIGQUIT))
		printf("\n SIGQUIT pending\n");

	sigdelset(&oldmask, SIGQUIT);
	Sigprocmask(SIG_SETMASK, &oldmask, NULL);
	printf("SIGQUIT unblocked\n");
	pr_mask("Main:");				
	sleep(10);							/*SIGQUIT here will terminate with core file*/
	
	return 0;
}

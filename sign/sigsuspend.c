#include "csapp.h"

void sig_int(int signo)
{
	pr_mask("\n in sig_int: ");
	return;
}

int main(int argc, char *argv[])
{
	sigset_t		newmask, oldmask, zeromask;
	Signal(SIGINT, sig_int);
	Sigemptyset(&zeromask);

	Sigemptyset(&newmask);

	Sigaddset(&newmask, SIGINT);
	Sigprocmask(SIG_BLOCK, &newmask, &oldmask);

	pr_mask("in critical reqion:");

	sigsuspend(&zeromask);
	pr_mask("after return from sigsuspend:");

	Sigprocmask(SIG_SETMASK, &oldmask, NULL);

	printf("Hello, world\n");
	
	return 0;
}

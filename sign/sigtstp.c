#include "csapp.h"

void sig_tstp(int signo)	/* signal handler for SIGTSTP*/
{
	sigset_t	mask;

	/*  ¡­¡­move cursor to lower left corner, reset tty mode ¡­¡­*/

	/* unblock SIGTSTP, since it's blocked while we're handling it*/
	sigemptyset(&mask);
	sigaddset(&mask, SIGTSTP);
	sigprocmask(SIG_UNBLOCK, &mask, NULL);

	Signal(SIGTSTP, SIG_DFL);		/* reset disposition to default*/
	Kill(getpid(), SIGTSTP);		/* and sent the signal to curself*/
	/* we won't return from the kill until we're continued*/
	Signal(SIGTSTP, sig_tstp);	/*reestablish signal handler*/

	/* .... reset tty mode , redrow screen...*/
	return;
}

int main(int argc, char *argv[])
{
	int		n;
	char	buf[MAXLINE];

	/*only catch SIGTSTP if we're running with a job-control shell*/
	if(signal(SIGTSTP,SIG_IGN)==SIG_DFL)
		Signal(SIGTSTP, sig_tstp);

	while((n= read(STDIN_FILENO, buf, MAXLINE)) > 0)
		if(write(STDOUT_FILENO, buf, n)!=n)
			err_sys("write error");

	return 0;
}

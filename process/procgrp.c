#include "csapp.h"

static void sig_hup(int);
static void pr_ids(char *);

int main(int argc, char *argv[])
{
	char	c;
	pid_t	pid;

	pr_ids("parent");
	if((pid = Fork()) > 0)
	{
		sleep(5);
		exit(0);
	}
	else{
		pr_ids("child");
		signal(SIGHUP, sig_hug);
		kill(getpid(), SIGTSTP);	//stop ourself
		pr_ids("child");			/*this prints only if we're continued*/
		if(read(0, &c, 1) != 1)
			printf("read error from control terminal, erro = %d\n", errno);
		exit(0);
	}
}

static void sig_hup(int signo)
{
	printf("SIGHUP received, pid = %d\n", getpid());
	return;
}

static void pr_ids(char *name)
{
	printf("%s: pid = %d, ppid = %d, pgrp = %d\n", name, getpid(), getppid(), getpgrp());
	fflush(stdout);
}


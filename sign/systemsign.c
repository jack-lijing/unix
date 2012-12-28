#include "csapp.h"

void sig_int(int signo)
{
	printf("caught SIGINT\n");
	return;
}

void sig_chld(int signo)
{
	printf("caught SIGCHLD\n");
	return;
}

void sig_usr1(int signo)
{
	printf("caught SIGUSR1\n");
	return;
}


int main(int argc, char *argv[])
{
	int status;
	Signal(SIGINT, sig_int);
	Signal(SIGCHLD, sig_chld);
	Signal(SIGUSR1, sig_usr1);
	if( (status = system("/bin/ed")) < 0)
		err_sys("system() error");
	printf("hello world\n");
	return 0;
}

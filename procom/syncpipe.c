#include "csapp.h"

static int pfd1[2], pfd2[2];

void TELL_WAIT(void)
{
	Pipe(pfd1);
	Pipe(pfd2);
}

void TELL_PARENT(pid_t pid)
{
	Rio_writen(pfd2[1], "c", 1);
}

void WAIT_PARENT(void)
{
	char	c;
	Rio_readn(pfd1[0],&c, 1);
	if(c != 'p')
		err_quit("WAIT_PARENT: incorrect data");
}

void TELL_CHILD(pid_t pid)
{
	Rio_writen(pfd1[1], "p", 1);
}

void WAIT_CHILD(void)
{
	char	c;
	Rio_readn(pfd2[0],&c, 1);
	if(c != 'c')
		err_quit("WAIT_CHILD: incorrect data");
}

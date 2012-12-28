#include "csapp.h"

static void charactatime(
/* the example show the race of process*/
int main(int argc, char *argv[])
{
	pid_t pid;

	if((pid = Fork()) == 0)
		charactatime("output from child\n");
	else
		charactatime("output from parent\n");
	
	return 0;
}

static void charactatime(char *str)
{
	char *ptr;
	int	c;
	setbuf(stdout, NULL)		/*set nobuffered*/
	for(ptr = str; c = *ptr++;)
		putc(c, stdout);
}

/*  following is no race version*/
/*
int main(int argc, char *argv[])
{
	pid_t pid;

	TELL_WAIT();
	if((pid = Fork()) == 0)
	{
		WAIT_PARENT();
		charactatime("output from child\n");
	}
	else
	{
		charactatime("output from parent\n");
		TELL_CHILD(pid);
	}

	return 0;
}
*/

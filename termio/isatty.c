#include "csapp.h"
#include <termios.h>

int isatty(int fd)
{
	struct termios term;
	return(tcgetattr(fd, &term) != -1);
}

int main(int argc, char *argv[])
{
	printf("fd0: %s\n", isatty(0)? "tty":"not a tty");
	printf("fd1: %s\n", isatty(1)? "tty":"not a tty");
	printf("fd2: %s\n", isatty(2)? "tty":"not a tty");
	return 0;
}

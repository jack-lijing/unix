#include "csapp.h"
#include <termios.h>

static void pr_winsize(int fd)
{
	struct winsize	size;
	if(ioctl(fd, TIOCGWINSZ, (char *) &size) < 0)
		err_sys("TIOCGWINSZ ERROR");
	printf("%d rows, %d columns\n", size.ws_row, size.ws_col);
}

static void sig_winch(int signo)
{
	printf("SIGWITNCH received \n");
	pr_winsize(STDIN_FILENO);
	return;
}

int main(int argc, char *argv[])
{
	if(isatty(STDIN_FILENO) ==0)
		exit(1);
	Signal(SIGWINCH, sig_winch);
	pr_winsize(STDIN_FILENO);		// print initial size
	for(;;)							// and sleep forever
		pause();
	printf("Hello, world\n");		
	
	return 0;
}

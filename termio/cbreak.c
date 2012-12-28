#include "csapp.h"
#include <termios.h>
#include <limits.h>

static struct termios	save_termios;
static int				ttysavefd = -1;
static enum {RESET, RAM, CBREAK} ttystate = RESET;

int tty_cbreak(int fd)		/* put terminal into a cbreak mode */
{
	struct termios	buf;

	if(tcgetattr(fd, &save_termios) < 0)
		return -1;

	buf = save_termios;		/* structure copy*/
	
	buf.c_lflag &= ~(ECHO | ICANON);	//echo off, canonical mode off
	buf.c_cc[VMIN] = 1;		/*Case B: 1 byte at a time, no timer*/
	buf.c_cc[VTIME] = 0;

	if(tcsetattr(fd, TCSAFLUSH, &buf) < 0)
		return -1;

	ttystate = CBREAK;
	ttysavefd = fd;
	return 0;
}

int tty_raw(int fd)		/* put terminal into a raw mode*/
{
	struct termios	buf;

	if(tcgetattr(fd, &save_termios) < 0)
		return -1;
	
	buf = save_termios;

	buf.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);	/*echo off, cannonical mode off, extended input processing off, signal chars off*/

	buf.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
					/*no sigint on break, CR-to-NL off, input parity check off, don't strip 8th bit on input, output flow control off*/
	buf.c_cflag &= ~(CSIZE | PARENB);
					/* clear size bits, parity checking off*/
	buf.c_cflag |= CS8;		/*set 8 bits/char */
	buf.c_oflag &= ~(OPOST);		/*output processing off*/

	buf.c_cc[VMIN] = 1;		/*case B: 1 bytes at a time, no timer*/
	buf.c_cc[VTIME] = 0;

	if(tcsetattr(fd, TCSAFLUSH, &buf) < 0)
		return -1;
	ttystate = RAM;
	ttysavefd = fd;
	return 0;
}

int tty_reset(int fd)		/*restore terminal's mode*/
{
	if(ttystate != CBREAK && ttystate != RAM)
		return 0;
	if(tcsetattr(fd, TCSAFLUSH, &save_termios) < 0)
		return -1;
	ttystate = RESET;
	return 0;
}

void tty_atexit(void)			/*can be set up by atexit(tty_atexit)*/
{
	if(ttysavefd >= 0)
		tty_reset(ttysavefd);
}

struct termios *tty_termios(void)		/*let caller see original tty state*/
{
	return(&save_termios);
}

static void sig_catch(int signo)
{
	printf("signal caught \n");
	tty_reset(STDIN_FILENO);
	exit(0);
}

int main(int argc, char *argv[])
{
	atexit(tty_atexit);
	int		i;
	char	c;
	Signal(SIGINT, sig_catch);
	Signal(SIGQUIT, sig_catch);
	Signal(SIGTERM, sig_catch);

	if(tty_raw(STDIN_FILENO) < 0)
		err_sys("tty_raw error");

	printf("Enter raw mode characters, terminate with DELETE\n");
	while ((i = read(STDIN_FILENO, &c, 1)) == 1)
	{
		if((c &= 255) == 0177)		/* 0177 = ASCII DELETE*/
			break;
		printf("%o\n", c );
	}
	if(tty_reset(STDIN_FILENO) < 0)
		err_sys("tty_reset error");
	if(i <= 0)
		err_sys("read error");
	if(tty_cbreak(STDIN_FILENO) < 0)
		err_sys("tty_raw error");
	printf("\n Enter cbreak mode characters, terminate with SIGINT\n");
	while ( (i = read(STDIN_FILENO, &c, 1)) == 1)
	{
		c &= 255;
		printf("%o\n", c);
	}
	tty_reset(STDIN_FILENO);
	if( i < 0)
		err_sys("read error");;

	return 0;
}

#include "csapp.h"
#include <termios.h>

#define		MAX_PASS_LEN	8

char *getpass(const char *prompt)
{
	static char		buf[MAX_PASS_LEN + 1];	// null byte at end
	char			*ptr;
	sigset_t		sig, sigsave;
	struct termios	term, termsave;
	FILE			*fp;
	int				c;

	if((fp = fopen(ctermid(NULL), "r+")) == NULL)
		return NULL;
	setbuf(fp, NULL);

	sigemptyset(&sig);		/* block sigint & sigtstp, save signal mask*/
	sigaddset(&sig, SIGINT);
	sigaddset(&sig, SIGTSTP);
	sigprocmask(SIG_BLOCK, &sig, &sigsave);

	tcgetattr(fileno(fp), &termsave);		/*save ttu state*/
	term = termsave;				//struct copy
	term.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);
	tcgetattr(fileno(fp), &term);

	fputs(prompt, fp);

	ptr = buf;
	while ((c = getc(fp)) != EOF && c!='\n')
	{
		if(ptr < &buf[MAX_PASS_LEN])
			*ptr++ = c;
	}
	*ptr = 0;			/*null terminate*/
	putc('\n', fp);		/*we echo a newline*/

						/*restore tty state*/
	tcsetattr(fileno(fp), TCSAFLUSH, &termsave);
	
						/*restore signal mask*/
	sigprocmask(SIG_SETMASK, &sigsave, NULL);
	fclose(fp);			/*done with /dev/tty */
	return buf;
}

int main(int argc, char *argv[])
{
	char	*ptr;
	if((ptr = getpass("Enter password:")) == NULL)
		err_sys("getpass error");
	printf("password: %s\n", ptr);

	/*now use password(probably encrypt it)*/

	while(*ptr != 0)
		*ptr++ = 0;

	return 0;
}

#include "csapp.h"
#include <setjmp.h>

static sigjmp_buf				jmpbuf;
static volatile sig_atomic_t	canjump;
void sig_usr1(int signo)
{
	time_t	starttime;

	if(canjump == 0)
		return;			/*unexpected signal ignore*/

	pr_mask("start sig_usr1: ");

	Alarm(3);

	starttime = time(NULL);
	for (; ; )		//busy wait for 5 seconds
	{
		if(time(NULL) > starttime +5 )
			break;
	}
	pr_mask("finish sig usr1: ");
	canjump = 0;
	siglongjmp(jmpbuf, 1);		/*jump back to main, don't return*/
}

void sig_alarm(int signo)
{
	pr_mask("is sig_alarm: ");
	return;
}

int main(int argc, char *argv[])
{
	Signal(SIGUSR1, sig_usr1);
	Signal(SIGALRM, sig_alarm);

	pr_mask("starting main: ");

	if(sigsetjmp(jmpbuf, 1))
	{
		pr_mask("ending main:");
		exit(0);
	}

	canjump = 1;	/* now sigsetjmp() is ok*/

	for (; ; )
	{
		pause();
	}
}

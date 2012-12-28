#include "csapp.h"

int daemon_init(void)
{
	pid_t	pid;
	if((pid = Fork()) != 0)
		exit(0);			/*parent goes byebye*/

	setsid();		/*become session leader*/

	chdir("/");		/* change working directory*/

	umask(0);		//cleat our file mode creation mask

	return 0;

}

int main(int argc, char *argv[])
{
	printf("Hello, world\n");
	daemon_init();
	while (1)
	{
		sleep(2);
	}
	return 0;
}

#include "csapp.h"

static int n = 0;

int main(int argc, char *argv[])
{
	pid_t pid;
	TELL_WAIT();
	pid = Fork();
	if (pid == 0)
	{	
		while ( n < 1000)
		{
			WAIT_PARENT();
			printf(" c%d ", n);
			n += 2;
			TELL_PARENT(getppid());
			//sleep(1);
		}
		
	}
	else if(pid > 0)
	{
		while (n < 1000)
		{
			
			printf(" p%d ", n);
			n += 2;
			TELL_CHILD(pid);
		//	sleep(1);
			WAIT_CHILD();
			
		}
		
	}
	printf("Hello, world\n");
	
	return 0;
}

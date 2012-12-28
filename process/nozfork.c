#include "csapp.h"
int main(int argc, char *argv[])
{
	pid_t pid;

	if((pid = Fork()) == 0)	//first child
	{
		pid = Fork();
		if(pid > 0)
			exit(0);	//parent from second child exit
		/*
		We/re the second child; our parent becomes init as soon as our real
		parent calls exit() in the statement above.
		here's where we's continue executing, knowing that when we're done
		init will reap our status
		*/
		sleep(2);
		printf("second child, parent pid = %d\n", getppid());
		exit(0);
	}
	if(waitpid(pid, NULL, 0) != pid)	//wait for first child
		err_sys("waitpid error");
	/*we are the parent( the original process); we continue executing,
	knowing that we're not the parent of the second child*/

	exit(0);
}

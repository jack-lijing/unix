#include "csapp.h"
#include <sys/mman.h>

#define	NLOOPS	1000
#define	SIZE	sizeof(long)	// size of shared memory area

int update(long *ptr)
{
	printf("%d ", *ptr);
	return ((*ptr)++);
}

int main(int argc, char *argv[])
{
	int		fd, i, counter;
	pid_t	pid;
	caddr_t	area;

	fd	= Open("/dev/zero", O_RDWR, 0);
	area = Mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(area == (caddr_t)-1)
		unix_error("mmap error");
	Close(fd);					/* can close /dev/zero now that it's mapped*/

	TELL_WAIT();
	pid = Fork();
	if(pid > 0)
	{
		for (i = 0; i< NLOOPS ; i += 2)
		{
			if((counter = update((long *)area)) != i)
				err_quit("parent: expected %d, got %d", i, counter);
			TELL_CHILD(pid);
			WAIT_CHILD();
		}
	}else
	{
		for (i = 1; i< NLOOPS +1 ; i += 2)
		{
			WAIT_PARENT();
			if((counter = update((long *)area)) != i)
				err_quit("child: expected %d, got %d", i, counter);
			TELL_PARENT(getppid());
		}
	}
	return 0;
}

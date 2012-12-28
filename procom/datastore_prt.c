#include "csapp.h"
#include <sys/ipc.h>
#include <sys/shm.h>

#define	ARRAY_SIZE	40000
#define	MALLOC_SIZE	100000
#define	SHM_SIZE	100000
#define	SHM_MODE	(SHM_R |SHM_W)	/*user read/ write*/

char	array[ARRAY_SIZE];		/*uninitialized data = bss*/

static int	a[10] = {0};
int main(int argc, char *argv[])
{
	int	shmid;
	char	*ptr, *shmptr;

	printf("a from %x to %x \n", &a[0], &a[9]);
	printf("array[] from %x to %x \n", &array[0], &array[ARRAY_SIZE]);
	printf("stack around %x \n", &shmid);
	
	ptr = Malloc(MALLOC_SIZE);
	printf("malloc from %x to %x\n", ptr, ptr + MALLOC_SIZE);
	if((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0)
		unix_error("shmat error");
	if ((shmptr = shmat(shmid, 0, 0)) == (void*)-1)
		unix_error("shmat error");
	printf("shared memorr attached  from %x to %x\n", shmptr, shmptr + SHM_SIZE);
	if(shmctl(shmid, IPC_RMID, 0) < 0)
		unix_error("shmctl error");
	return 0;
}

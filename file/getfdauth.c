#include "ourhdr.h"
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int accmode, val;
	if(argc != 2)
		err_quit("usage: %s <descriptor>", argv[0]);
	if((val = fcntl(atoi(argv[1]),F_GETFL, 0)) < 0)
		err_sys("fcntl error for fd %d", atoi(argv[1]));
	accmode = val & O_ACCMODE;
	if(accmode == O_RDONLY)
		printf("read only");
	else if	(accmode == O_WRONLY)	printf("write only");
	else if	(accmode == O_RDWR)	printf("write read");
	else err_dump("unknwon access mode");

	if(val & O_APPEND)			printf(", append");
	if(val & O_NONBLOCK)		printf(", nonblocking");
//#if !define(_POSIX_SOURCE) && define(O_SYNC)		
	if (val & O_SYNC)printf(", synchronous writes");
//#endif
	putchar('\n');

	return 0;
}


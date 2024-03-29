#include "csapp.h"

char	buf[10000000];


int main(int argc, char *argv[])
{
	int		ntowrite, nwrite;
	char	*ptr;

	ntowrite = read(STDIN_FILENO, buf, sizeof(buf));
	fprintf(stderr, "read %d bytes\n", ntowrite);

	set_fl(STDOUT_FILENO, O_NONBLOCK);		/*	set nonblocking	*/

	for (ptr = buf; ntowrite > 0; )
	{
		errno = 0;
		nwrite = write(STDOUT_FILENO, ptr, ntowrite);
		fprintf(stderr, "nwrite = %d, errno = %d \n", nwrite, errno);
		if(nwrite > 0)
		{
			ntowrite -= nwrite;
			ptr += nwrite;
		}
	}
	clr_fl(STDOUT_FILENO, O_NONBLOCK);
	return 0;
}

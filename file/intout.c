#include "ourhdr.h"
#define BUFFSIZE	8192


int main(int argc, char *argv[])
{
	int	n;
	char	buf[BUFFSIZE];

	while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
		if(write(STDOUT_FILENO, buf, n) != n)
			err_sys("write error");
	if(n < 0)
		err_sys("read error");
	return 0;
}

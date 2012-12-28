#include "csapp.h"

int main(int argc, char *argv[])
{
	int		n, fd[2];
	pid_t	pid;
	char	line[MAXLINE];

	Pipe(fd);
	pid = Fork();

	if(pid > 0){
		Close(fd[0]);
		Rio_writen(fd[1], "hello world\n", 12);
	}else				/*child*/
	{
		Close(fd[1]);
		n = Rio_readn(fd[0], line, MAXLINE);
		Rio_writen(STDOUT_FILENO, "child print\n", 12);
		Rio_writen(STDOUT_FILENO, line, n);
	}
	return 0;
}

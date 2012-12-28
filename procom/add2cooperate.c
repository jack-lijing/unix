#include "csapp.h"

#define SHELL		"/bin/sh"

void sig_pipe(int signo)
{
	printf("SIGPIPE caught \n");
	exit(1);
}

int main(int argc, char *argv[])
{
	int n, fd1[2], fd2[2];
	pid_t	pid;
	char	line[MAXLINE];

	Signal(SIGPIPE, sig_pipe);
	Pipe(fd1);
	Pipe(fd2);
	pid = Fork();
	if(pid > 0 )			//parent
	{
		Close(fd1[0]);
		Close(fd2[1]);
		printf("cooperate input:\n");
		while (fgets(line, MAXLINE, stdin) != NULL)
		{
			
			n = strlen(line);
			Rio_writen(fd1[1], line, n);
		//	printf("%s %d", line, n);
			n = read(fd2[0], line, MAXLINE);
			if( n == 0)
				err_msg("child closed pipe");
			break;
		}
		line[n] = 0;
		Fputs(line, stdout);
		exit(0);
	}
	else				// child
	{
		Close(fd1[1]);
		Close(fd2[0]);
		if (fd1[0] != STDIN_FILENO)
		{
			Dup2(fd1[0], STDIN_FILENO);
			Close(fd1[0]);
		}
		if (fd2[1] != STDOUT_FILENO)
		{
			Dup2(fd2[1], STDOUT_FILENO);
			Close(fd2[1]);
		}
		if(execl(SHELL, "sh", "-c", "./add2", "add2",(char *)0) < 0)
			err_sys("execl error");
	}
	printf("Hello, world\n");
	
	return 0;
}

#include "csapp.h"



static void sig_pipe(int signo)
{
	printf("SIGPIPE caught\n");
}

int main(int argc, char *argv[])
{
	int	n, fd[2];
	pid_t	pid;
	char	line[MAXLINE];

	Signal(SIGPIPE, sig_pipe);
	if(s_pipe(fd) < 0)
		unix_error("s_pipe error");

	pid = Fork();
	if(pid > 0 )			//parent
	{
		Close(fd[1]);
		printf("cooperate input:\n");
		while (fgets(line, MAXLINE, stdin) != NULL)
		{
			
			n = strlen(line);
			Rio_writen(fd[0], line, n);
		//	printf("%s %d", line, n);
			n = read(fd[0], line, MAXLINE);
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
		Close(fd[0]);
		if (fd[1] != STDIN_FILENO)
		{
			Dup2(fd[1], STDIN_FILENO);
		}
		if (fd[1] != STDOUT_FILENO)
		{
			Dup2(fd[1], STDOUT_FILENO);
		}
		if(execl("./add2", "add2",(char *)0) < 0)
			err_sys("execl error");
	}
	printf("Hello, world\n");
	
	return 0;
}

#include "csapp.h"

#define SHELL		"/bin/sh"
#define DEF_PAGER	"/usr/bin/more"

int main(int argc, char *argv[])
{
	int n, fd[2];
	pid_t	pid;
	char	line[MAXLINE], *pager, *argv0;
	FILE	*fp;

	if(argc != 2)
		err_quit("usage: %s <pathname>", argv[0]);

	fp = Fopen(argv[1], "r");
	Pipe(fd);
	if ((pid = Fork()) > 0)
	{
		Close(fd[0]);
		while (Fgets(line, MAXLINE, fp) != NULL)
		{
			n = strlen(line);
			Rio_writen(fd[1], line, n);
		}
	
		Close(fd[1]);		// close write end of pipe for reader
		Waitpid(pid, NULL, 0);
		exit(0);
	}
	else
	{
		Close(fd[1]);
		if (fd[0] != STDIN_FILENO)
		{
			Dup2(fd[0], STDIN_FILENO);
			Close(fd[0]);
		}

		/*get arguments for execl()*/
		pager = Getenv("PAGER");
		if(pager == NULL)
			pager = DEF_PAGER;
		/*
		if((argv0 = strrchr(pager, '/')) != NULL)
			argv0++;			//*step past rightmost slash
		else
			argv0 = pager;		// no slash in pager
			*/
		if(execl(SHELL, "sh", "-c",pager, argv[1], (char *)0) < 0)
			err_sys("execl error for %s", pager);
	}
	
	return 0;
}

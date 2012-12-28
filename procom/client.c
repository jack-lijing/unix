#include "open.h"

/*Open the file by sending the "name" and "oflag" to the connection server and reading a file descriptor back*/
int csopen(char *name, int oflag)
{
	pid_t			pid;
	int				len;
	char			buf[10];
	struct iovec	iov[3];
	static int		fd[2] = {-1, -1};

	if (fd[0] < 0)	// fork/exec our open server first time
	{
		if(s_pipe(fd) < 0)
			err_sys("s_pipe error");
		if((pid = Fork()) == 0)
		{
			Close(fd[0]);
			if(fd[1] != STDIN_FILENO)
				Dup2(fd[1], STDIN_FILENO);
			//if(fd[1] != STDOUT_FILENO)
			//	Dup2(fd[1], STDOUT_FILENO);
			if(execl("./opend", "opend", NULL) < 0)
				err_sys("execl error");
		}
		Close(fd[1]);			// parent
	}
	sprintf(buf, " %d", oflag );		// oflag to ascii
	iov[0].iov_base	= CL_OPEN " ";
	iov[0].iov_len	= strlen(CL_OPEN) + 1;
	iov[1].iov_base	= name;
	iov[1].iov_len	= strlen(name);
	iov[2].iov_base	= buf;
	iov[2].iov_len	= strlen(buf) + 1;	// + 1 for null at end of buf
	len = iov[0].iov_len + iov[1].iov_len + iov[2].iov_len;
	if(writev(fd[0], &iov[0], 3) != len)
		err_sys("writev error");

	/* read descriptor, returned errors handled by write() */
	return(recv_fd(fd[0], write));
}

int main(int argc, char *argv[])
{
	int		n,fd;
	char	buf[BUFFSIZE], line[MAXLINE];

	/* read filename to cat from stdin */
	while (fgets(line, MAXLINE, stdin) != NULL)
	{
		line[strlen(line) - 1] = 0;
		if((fd = csopen(line, O_RDONLY)) < 0)
			continue;

		while ((n = read(fd, buf, BUFFSIZE)) > 0)
		{
			if(write(STDOUT_FILENO, buf, n) != n)
				err_sys("write error");
		}
		if(n < 0)
			err_sys("read error");
		close(fd);
	}
	return 0;
}

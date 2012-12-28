#include "open.h"

int		debug;

/*	Open the file by sending the "name" and "oflag" to the connection server and reading a file descriptor back */
csopen(char *name, int oflag)
{
	int				len;
	char			buf[10];
	struct iovec	iov[3];
	static int		csfd = -1;

	if (csfd < 0)
	{
		if((csfd = cli_conn(CS_OPEN)) < 0)		//open connection to conn server
			err_sys("cli_conn error");
	}

	sprintf(buf, " %d", oflag );		// oflag to ascii
	iov[0].iov_base	= CL_OPEN " ";
	iov[0].iov_len	= strlen(CL_OPEN) + 1;
	iov[1].iov_base	= name;
	iov[1].iov_len	= strlen(name);
	iov[2].iov_base	= buf;
	iov[2].iov_len	= strlen(buf) + 1;	// + 1 for null at end of buf
	len = iov[0].iov_len + iov[1].iov_len + iov[2].iov_len;
	if(writev(csfd, &iov[0], 3) != len)
		err_sys("writev error");

	/* read descriptor, returned errors handled by write() */
	return(recv_fd(csfd, write));

}


int main(int argc, char *argv[])
{
	int		n,fd;
	char	buf[BUFFSIZE], line[MAXLINE];

	/* read filename to cat from stdin */
	while (fgets(line, MAXLINE, stdin) != NULL)
	{
		line[strlen(line) - 1] = 0;
		if((fd = csopen(line, O_RDONLY)) < 0)		//open file RDONLY  0x0000
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

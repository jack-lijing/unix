#include "csapp.h"

#define PIDFILE		"daemon.pid"

int main(int argc, char *argv[])
{
	int		fd, val;
	char	buf[10];

	fd = Open(PIDFILE, O_WRONLY | O_CREAT, FILE_MODE);

	/*try and set a write lock on the entire file*/
	if(write_lock(fd, 0, SEEK_SET, 0) < 0)
	{
		if(errno == EACCES || errno == EAGAIN)
		{
			printf("there is a daemoon is running, pls close it first\n");
			exit(0);			/*gracefully exit, daemon is alreay running*/
		}
		else
			err_sys("write_lock error");
	}

	/* truncate to zero length. now that we have the lock */
	if(ftruncate(fd, 0) < 0)
		err_sys("ftruncate error");

	/*and write our process ID*/
	sprintf(buf, "%d\n", getpid());
	if(write(fd, buf, strlen(buf)) != strlen(buf))
		err_sys("write error");
	
		/* set close-no-exec flag for descriptor*/
	if((val = fcntl(fd, F_GETFD, 0)) < 0)
		err_sys("fcntl F_GETFD error");
	val |= FD_CLOEXEC;
	if(fcntl(fd, F_SETFD, val) < 0)
		err_sys("fcntl F_SETFD error");

	/*leave file open until we terminate: lock wiil be held*/
	sleep(20);
	/*do whatever...*/

	return 0;
}

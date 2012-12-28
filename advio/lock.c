#include "csapp.h"

pid_t look_test(int fd, int type, off_t offset, int whence, off_t len)
{
	struct flock		lock;

	lock.l_type		= type;
	lock.l_start	= offset;
	lock.l_whence	= whence;
	lock.l_len		= len;

	if(fcntl(fd, F_GETLK, &lock) < 0)
		err_sys("fctnl error");
	if(lock.l_type == F_UNLCK)
		return(0);
	return lock.l_pid;
}

void lockabyte(const char *name, int fd, off_t offset)
{
	if(writew_lock(fd, offset, SEEK_SET, 1) < 0)
		err_sys("%s: writew_lock error\n", name);
	printf("%s, got the lock, byte %d\n", name, offset);
}



int main(int argc, char *argv[])
{
	int		fd;
	pid_t	pid;

	/*Create a file and write two bytes to it*/
	fd = Open("templock", O_RDWR | O_CREAT | O_TRUNC, FILE_MODE) ;

	if(write(fd, "ab", 2) != 2)
		err_sys("write error");

	TELL_WAIT();
	pid = Fork();

	if(pid == 0)
	{
		lockabyte("child", fd, 0);
		TELL_PARENT(getppid());
		WAIT_PARENT();
		lockabyte("child", fd, 1);
	}else 
	{
		lockabyte("parent", fd, 1);
		TELL_CHILD(pid);
		WAIT_CHILD();
		lockabyte("parent", fd, 0);
	}
	return 0;
}

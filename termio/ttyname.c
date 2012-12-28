#include "csapp.h"
#include <dirent.h>
#include <termios.h>
#include <limits.h>

#define	DEV		"/dev/"
#define	DEVLEN	sizeof(DEV)-1

char *ttyname(int fd)
{
	struct stat		fdstat,	devstat;
	DIR				*dp;
	struct dirent	*dirp;
	static char		pathname[_POSIX_PATH_MAX + 1];
	char			*rval;

	if(isatty(fd) == 0)
		return(NULL);
	if(fstat(fd, &fdstat) < 0)
		return(NULL);
	if(S_ISCHR(fdstat.st_mode) == 0)
		return(NULL);

	strcpy(pathname, DEV);
	if((dp = opendir(DEV)) == NULL)
		return NULL;
	rval = NULL;
	while ((dirp =  readdir(dp)) != NULL)
	{
		if(dirp->d_ino != fdstat.st_ino)
			continue;						/* fast test to skip most entries*/
		strncpy(pathname + DEVLEN, dirp->d_name, _POSIX_PATH_MAX - DEVLEN);
		if(stat(pathname, &devstat) < 0)
			continue;
		if(devstat.st_ino == fdstat.st_ino && devstat.st_dev == fdstat.st_dev)	/*find a  match*/
		{
			rval = pathname;
			break;
		}
	}
	closedir(dp);
	return rval;
}

int main(int argc, char *argv[])
{
	printf("fd0: %s\n", isatty(0)? ttyname(0):"not a tty");
	printf("fd1: %s\n", isatty(1)? ttyname(1):"not a tty");
	printf("fd2: %s\n", isatty(2)? ttyname(2):"not a tty");
	return 0;
}

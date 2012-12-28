#include <sys/stat.h>
#include <fcntl.h>
#include "ourhdr.h"

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

int main(int argc, char *argv[])
{
	int fd;
	if((fd = open("file.hole", O_WRONLY | O_CREAT | O_TRUNC , FILE_MODE)) < 0)
		err_sys("create file error");
	if(write(fd, buf1, 10) != 10)
		err_sys("write buf error");
	/*offset now = 10*/

	if(lseek(fd, 40, SEEK_SET) == -1)
		err_sys("lseek error");
	/*offset now = 40*/

	if(write(fd, buf2, 10) != 10)
		err_sys("write buf2 error");
	/*offset now = 50*/
	
	exit(0);
}

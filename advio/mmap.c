#include "csapp.h"
#include <sys/mman.h>


int main(int argc, char *argv[])
{
	int		fdin, fdout;
	char	*src, *dst;

	struct stat statbuf;

	if(argc != 3)
		err_quit("usage: %s <fromfile> <tofile>", argv[0]);
	
	fdin	= Open(argv[1], O_RDONLY, NULL);

	fdout	= Open(argv[2], O_RDWR | O_CREAT | O_TRUNC, NULL);
	Fstat(fdin, &statbuf);							/*need size of input file*/
	Lseek(fdout, statbuf.st_size -1, SEEK_SET);		/*set size of output file*/
	Rio_writen(fdout, "", 1);

	if((src = mmap(0, statbuf.st_size, PROT_READ, MAP_FILE | MAP_SHARED, fdin, 0)) == (caddr_t)-1)
		err_sys("mmap error for input");
	if((dst = mmap(0, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, fdout, 0)) == (caddr_t)-1)
		err_sys("mmap error for output");
	
	memcpy(dst, src, statbuf.st_size);		/*does the file copy*/
	return 0;
}

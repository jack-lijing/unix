#include "ourhdr.h"

void pr_stdio(const char *, FILE *);

int main(int argc, char *argv[])
{
	FILE	*fp;

	fputs("enter any character\n", stdout);
	if(getchar() == EOF)
		err_sys("getchar error");
	fputs("one line to standard error\n", stderr);

	pr_stdio("stdin", stdin);
	pr_stdio("stdout", stdout);
	pr_stdio("stderr", stderr);

	if((fp = fopen("/etc/motd", "r")) == NULL)
		err_sys("fopen error");
	if(getc(fp) == EOF)				//alloc buffer first time
		err_sys("getc error");
	
	pr_stdio("/etc/motd", fp);
	
	return 0;
}

void pr_stdio(const char *name, FILE *fp)
{
	printf("stream = %s ", name);

	if		(fp->_flags & _IONBF)	printf("unbuffered");
	else if	(fp->_flags & _IOLBF)	printf("line buffered");
	else if	(fp->_flags & _IOFBF)	printf("fully buffered");

	printf(", buffer size = %d \n", fp->_bf._size);
}

#include "csapp.h"

#define	PAGER	"${PAGER:-more}"

int main(int argc, char *argv[])
{
	char	line[MAXLINE];
	FILE	*fpin, *fpout;

	
	if(argc != 2)
		err_quit("usage: %s <pathname>", argv[0]);

	fpin = Fopen(argv[1], "r");

	fpout = Popen(PAGER, "w");

	/* copy argv[1] to  pager*/
	while (Fgets(line, MAXLINE, fpin) != NULL)
	{
		Fputs(line, fpout);
	}
	Pclose(fpout);
	return 0;
}

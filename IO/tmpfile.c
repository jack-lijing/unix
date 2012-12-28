#include "ourhdr.h"

int main(int argc, char *argv[])
{
	char	name[L_tmpnam], line[MAXLINE];
	FILE	*fp;

	/*tmpnam() possibly used unsafely; consider using mkstemp()*/
	printf("%s\n", tmpnam(NULL));	//first temp name

	tmpnam(name);					//second temp name
	printf("%s\n", name);

	if((fp = tmpfile()) == NULL)	//create temp file
		err_sys("tmpfile error");
	fputs("one line of output\n", fp);	//write to temp file
	rewind(fp);				//then read it back

	if(fgets(line, sizeof(line), fp) == NULL)
		err_sys("fgets error");
	fputs(line, stdout);	//print the line we wrote
	return 0;
}

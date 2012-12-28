#include "csapp.h"

int main(int argc, char *argv[])
{
	int m, n, int1, int2;
	char	line[MAXLINE];
	printf("add input:\n");
	while ((n = read(STDIN_FILENO, line, MAXLINE)) > 0)
	{
		line[n] = '\0';
		if ((m = sscanf(line, "%d%d", &int1, &int2)) == 2)
		{
			sprintf(line, "%d\n", int1 + int2);
			n = strlen(line);
			printf("add print:%d\n", n);
			Rio_writen(STDOUT_FILENO, line, n);
		}
		else
		{
			Rio_writen(STDOUT_FILENO, "invalid args\n", 13);
		}
	}
	return 0;
}

#include <stdio.h>
typedef char* CP;
typedef PC char;
int main(int argc, char *argv[])
{
	CP cp= "char * CP";
	PC pc= "CP char *"

	printf("%s %s\n", cp, pc);
	
	return 0;
}

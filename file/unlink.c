#include "ourhdr.h"

int main(int argc, char *argv[])
{
	if(open("tempfile", O_RDWR) < 0)
		err_sys("open tempfile error");
	if(unlink("tempfile") < 0)
		err_sys("unlink error");
	printf("file unlinked\n");
	sleep(15);
	printf("done\n");
	
	return 0;
}

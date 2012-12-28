#include	"csapp.h"
#include	<sys/uio.h>

#define CL_OPEN	"open"		// client's request for server
#define CS_OPEN "/home/stevens/opend"		//server's well-known name

int csopen(char *, int);

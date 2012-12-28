#include "opend.h"

extern int send_fd(int clifd, int fd);

char	errmsg[MAXLINE];	//error message string to return to client
int		oflag;		//open() flag: O_xxxx ....
char	*pathname;	// of file to open() for client

#define	MAXARGC		50
#define	WHITE		"\t\n"	

/* This function is called by buf_args(), which is called by request(), buf_args() has broken up the client's buffer 
into an argv[] style array, which we now process*/
/*	INPUT
		argc	: number of input parameter(should be 3)	
		argv	: value of parameter	
	Return
		Success	: 0 (pathname will assignned by argv[1]; oflag will assignned by argv[2] )
		Failed	: -1
*/
int cli_args(int argc, char **argv)
{
	if (argc != 3 || strcmp(argv[0], CL_OPEN) != 0)
	{
		strcpy(errmsg, "usage:<pathname> <oflag>\n");
		return -1;
	}

	pathname = argv[1];	// save ptr to pathname to open
	oflag	= atoi(argv[2]);
	return 0;
}

/* buf[] contains white-space separated arguments. We convert it to an argv[] style array of pointers, and call the user's function (*optfunc)()
	to process the argv[] array. We return -1 to the caller if there' s aproblem parsing buf, else we return whaterver optfunc() returns.
	Note that user's buf[] array is modified(nulls placed after each token)*/
int buf_args(char *buf, int (*optfunc)(int, char **))
{
	char		*ptr, *argv[MAXARGC];
	int			argc;

	if(strtok(buf, WHITE) == NULL)		// an argv[0] is required
		return	-1;
	argc = 0;
	argv[0] = buf;
	while ((ptr = strtok(NULL, WHITE)) != NULL)
	{
		if(++argc >= MAXARGC-1)	// -1 for room for NULL at end
			return -1;
		argv[argc] = ptr;
	}
	argv[++argc] = NULL;
	return (*optfunc)(argc, argv);
	/* Since argv[] pointers point into the user's buf[], user's function can just copy the pointers,
	even though argv[] array will disappear on return*/
}

/*
Input
	buf		: msg server received(should be end by 0)
	nread	: the lenth of msg;
	fd		: the destination( file descriptor )  of newfd will be send to
*/

void request(char *buf, int nread, int fd)
{
	int		newfd;
	if (buf[nread-1] != 0 )
	{
		sprintf(errmsg, "request not null terminated: %*.*s\n", nread, nread, buf);
		send_err(fd, -1, errmsg);
		return;
	}

	/*parse the arguments , set options*/
	if (buf_args(buf, cli_args) < 0)
	{
		send_err(fd, -1, errmsg);
		return;
	}

	if ((newfd = open(pathname, oflag)) < 0)
	{
		sprintf(errmsg, "Can't open %s: %s\n", pathname, strerror(errno));
		send_err(fd, -1, errmsg);
		return;
	}

	/*send the descriptor*/
	if(send_fd(fd, newfd) < 0)
		err_sys("send_fd error");
	Close(newfd);
}

int main(int argc, char *argv[])
{
	int		nread;
	char	buf[MAXLINE];

	for (; ; )		// read arg buffer from client , process request
	{
		if((nread = read(STDIN_FILENO, buf, MAXLINE)) < 0)
			err_sys("read error on stream pipe");
		else if (nread == 0)
			break;		//client has closed the stream pipe
		
		//printf("buf: %s\n",buf);
		request(buf, nread, STDOUT_FILENO);
	}
	return 0;
}

#include "opend.h"

#define	NALLOC		10

static void client_alloc(void)	//alloc more entries in the client[] array
{
	int		i;
	if(client == NULL)
		client = malloc(NALLOC *sizeof(Client));
	else
		client = realloc(client, (client_size + NALLOC)*sizeof(Client));
	if(client == NULL)
		err_sys("can't alloc for client array");

	/* have to initialize the new entries */
	for(i = client_size; i< client_size + NALLOC; i++)
		client[i].fd = -1;		//fd of -1 means entry available
	client_size += NALLOC;
}

/* Called by loop() when connection request from a new client arrives*/
int client_add(int	fd, uid_t	uid)
{
	int		i;
	if(client == NULL)
		client_alloc();
again:
	for (i = 0; i < client_size ; i++)
	{
		if (client[i].fd == -1)		//find an available entry
		{
			client[i].fd	= fd;
			client[i].uid	= uid;
			return i;					//return index in client array
		}
	}

	/* client array full, time to realloc for more */
	client_alloc();
	goto	again;		// and search again(will work this time)
}

/* Called by loop() when we're donw with a client */
void client_del(int fd)
{
	int			i;
	for ( i = 0; i < client_size ; i++)
	{
		if(client[i].fd == fd)
		{
			client[i].fd = -1;
			return;
		}
	}
	log_quit("can't find client entry for fd %d", fd);
}

int		debug;
char	errmsg[MAXLINE];	//error message string to return to client
int		oflag;		//open() flag: 0xxxxx ....   O_RDONLY = 0x0000
char	*pathname;	// of file to open() for client
Client	*client = NULL;
int		client_size;

#define	MAXARGC		50
#define	WHITE		" \t\n"	

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
		strcpy(errmsg, "usage:<pathname>\n");
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
	char		*ptr, *argv[MAXARGC], *q;
	int			argc;

	int			len = strlen(buf);
	printf("len[%d]\n",len);
	argc = 0;
	while(q = strsep(&buf, WHITE))
	{
		printf("argv[%d]:%s\n", argc, q);
		argv[argc++] = q;
	}

	if (argc != 3 || strcmp(argv[0], CL_OPEN) != 0)
	{
		strcpy(errmsg, "usage:<pathname>\n");
		return -1;
	}


	pathname = argv[1];	// save ptr to pathname to open
	oflag	= atoi(argv[2]);
	return 0;
/*   strtok is obsoleted , pls use strsep;
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
	*/
	/* Since argv[] pointers point into the user's buf[], user's function can just copy the pointers,
	even though argv[] array will disappear on return*/
}


void request_sock( char *buf, int nread, int clifd, uid_t uid)
{
	log_msg("request msg:%s, from uid %d", buf, uid);

	int		newfd;
	if (buf[nread-1] != 0 )
	{
		sprintf(errmsg, "request from uid %d not null terminated: %*.*s\n",uid, nread, nread, buf);
		send_err(clifd, -1, errmsg);
		return;
	}
	
	log_msg("request:%s, from uid %d", buf, uid);

	/*parse the arguments , set options*/
	if (buf_args(buf, cli_args) < 0)
	{
		send_err(clifd, -1, errmsg);
		log_msg(errmsg);
		return;
	}

	if ((newfd = open(pathname, oflag)) < 0)
	{
		sprintf(errmsg, "Can't open %s: %s\n", pathname, strerror(errno));
		send_err(clifd, -1, errmsg);
		log_msg(errmsg);
		return;
	}

	/*send the descriptor*/
	if(send_fd(clifd, newfd) < 0)
		log_sys("send_fd error");
	log_msg("send fd %d over fd %d for %s", newfd, clifd, pathname);
	Close(newfd);
}


/* loop implement by select() */
void loop(void)
{
	int			i, n, maxfd, maxi, listenfd, clifd, nread;
	char		buf[MAXLINE];
	uid_t		uid;
	fd_set		rset, allset;

	FD_ZERO(&allset);

	// obtain fd to listen for client requests on
	if((listenfd = serv_listen(CS_OPEN)) < 0)
		log_sys("serv_listen error");
	FD_SET(listenfd, &allset);
	maxfd = listenfd;
	maxi = -1;

	for (; ; )
	{
		rset = allset;			//rset gets modified each time around
		if((n = select(maxfd + 1, &rset, NULL, NULL, NULL)) < 0)
			log_sys("select error");

		if (FD_ISSET(listenfd, &rset))
		{
			// accept new client request
			if((clifd = serv_accept(listenfd, &uid)) < 0)
				log_sys("serv_accept error: %d", clifd);
			i = client_add(clifd, uid);
			FD_SET(clifd, &allset);
			if(clifd > maxfd)
				maxfd = clifd;		// max index in client[] array
			if(i > maxi)
				maxi = i;			//max index in client[] array
			log_msg("new connection: uid %d, fd %d", uid, clifd);
			continue;
		}

		for ( i = 0; i <= maxi ; i++)		//go through client[] array
		{
			if((clifd = client[i].fd) < 0)
				continue;
			if(FD_ISSET(clifd, &rset))
			{
				// read argument buffer from client
				if((nread = read(clifd, buf, MAXLINE)) < 0)
					log_sys("read error on fd %d", clifd);
				else if (nread == 0)
				{
					log_msg("closed: uid %d, fd %d", client[i].uid, clifd);
					client_del(clifd);				//client has closed conn
					FD_CLR(clifd, &allset);
					Close(clifd);
				}else			// process client's request
					request_sock(buf, nread, clifd, client[i].uid);
			}
		}
	}
}


int daemon_init(void)
{
	pid_t	pid;
	if((pid = Fork()) != 0)
		exit(0);			/*parent goes byebye*/

	setsid();		/*become session leader*/

	chdir("/");		/* change working directory*/

	umask(0);		//cleat our file mode creation mask

	return 0;

}

int main(int argc, char *argv[])
{
	int		c;
	log_open("open.serv", LOG_PID, LOG_USER);

	opterr = 0;		//don't want getopt() writing to stderr
	while ((c = getopt(argc, argv, "d")) != EOF )
	{
		switch(c)
		{
			case 'd':		//debug
				debug = 1;
				break;

			case '?':
				err_quit("unrecognized option : -%c", optopt);
		}
	}
	if(debug == 0)
		daemon_init();
	loop();
}



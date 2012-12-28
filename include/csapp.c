#include "csapp.h"

/*********************************************************
			Error-handling functions
*********************************************************/
void unix_error(char *msg)
{
	fprintf(stdout, "%s: %s", msg, strerror(errno));
	exit(0);
}

void posix_error(int code, char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, strerror(code));
	exit(0);
}

void dns_error(char *msg)
{
	fprintf(stderr,"%s:DNS Error %d", msg, h_errno);
	exit(0);
}

void app_error(char *msg)
{
	fprintf(stderr, "%s", msg);
	exit(0);
}

void app_warn(char *msg)
{
	fprintf(stderr, "%s", msg);
	return;
}

/********************************************************
*	Wrappers for Unix process control functions *
*********************************************************/
pid_t Fork(void)
{
	pid_t pid;	
	if( (pid = fork()) < 0 )
		unix_error("Fork error");
	return pid;
}

pid_t Wait(int *status)
{
	pid_t rt = wait(status);
	if (rt < 0)
	{
		unix_error("Wait error");
	}
	return rt;
}

pid_t Waitpid(pid_t pid, int *iptr, int options)
{
	pid_t retpid;
	if((retpid = waitpid(pid, iptr, options)) < 0)
		unix_error("Waitpid error");
	return retpid;
}

void Pause()
{
	pause();
	return;
}

void Kill(pid_t p, int sig)
{
	int rec;
	rec = kill(p, sig);
	if (rec > 0)
	{
		unix_error("kill error");
	}
}

void Execve(const char*filename, char *const argv[], char *const envp[])
{
	int rt = execve(filename, argv, envp);
	if (rt == -1)
	{
		unix_error("Execve error");
	}
}

unsigned int Sleep(unsigned int sec)
{
	return sleep(sec);
}

unsigned int Alarm(unsigned int seconds)
{
	return alarm(seconds);
}

int System(char *cmd)
{
	int rt = system(cmd);
	if(rt < 0)
		unix_error("System error");
	return rt;
}

char *Getenv(const char *name)
{
	char *rt = getenv(name);
	if( NULL == rt)
		app_warn("warning:Getenv, no such value  ");
	return rt;
}

/* This interface is obsoleted by getrusage(2) and gettimeofday(2)*/
clock_t Times(struct tms *ts)
{
	clock_t rt = times(ts);
	if(rt < 0)
		unix_error("Times error");
}

long Sysconf(int name)
{
	int rt = sysconf(name);
	if(rt == -1)
		unix_error("Sysconf error");
	return rt;
}

/*********************************************************
*	wrappers for Unix communication between process*
*******************************************************/
void Pipe(int fildes[2])
{
	if(pipe(fildes) == -1)
		unix_error("Pipe error");
}

FILE *Popen(const char *cmdstring, const char *type)
{
	FILE *fp = popen(cmdstring, type);
	if(NULL == fp)
		unix_error("Popen error");
}

void Pclose(FILE *fp)
{
	if(pclose(fp) == -1)
		unix_error("Pclose error");
}

/*********************************************************
*	wrappers for Unix signal functions*
*******************************************************/
handler_t *Signal(int signum, handler_t *handler)
{
	struct sigaction action, old_action;
	action.sa_handler = handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags =SA_RESTART;
	if (sigaction(signum, &action, &old_action) < 0)
	{
		unix_error("Signal error");
	}
	return (old_action.sa_handler);
}

void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset)
{
	if(sigprocmask(how, set, oldset) < 0)
		unix_error("sigprocmask error");
	return;
}

void Sigemptyset(sigset_t *set)
{
	if(sigemptyset(set) < 0)
		unix_error("sigemptyset error");
	return;
}

void Sigaddset(sigset_t *set, int signum)
{
	if(sigaddset(set, signum) < 0)
		unix_error("sigaddset error");
	return;
}
/*************************************************
	*Wrappers for Unix I/O routines*
***************************************************/
int Open(const char *pathname, int flags, mode_t mode)
{
	int rt = open(pathname, flags, mode);
	if(rt < 0)
		unix_error("Open error");
	return rt;
}

ssize_t Read(int fd, void *buf, size_t count)
{
	ssize_t rt = read(fd, buf, count);
	if(rt < 0)
		unix_error("read error");
	return rt;
}

ssize_t Write(int fd, const void *buf, size_t count)
{
	ssize_t rt = write(fd, buf, count);
	if(rt < 0)
		unix_error("Write error");
	return rt;
}

off_t Lseek(int fildes, off_t offset, int whence)
{
	off_t rt;
	rt = lseek(fildes, offset, whence);
	if(rt < 0)
		unix_error("Lseek error");
	return rt;
}

int Dup2(int oldd, int newd)
{
	int n;
	n = dup2(oldd, newd);
	if( n < 0)
		unix_error("Dup2 error");
	return n;
}

void Fstat(int fd, struct stat *buf)
{
	if(fstat(fd, buf) < 0)
		unix_error("Fstat error");
}

void Close(int connfd)
{
	if (close(connfd) == -1)
	{
		unix_error("Close error");
	}
}

int Select(int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
{
	int rt;
	rt = select(n, readfds, writefds, exceptfds, timeout);
	if(rt == -1)
		unix_error("Select Error");
	return rt;
}

/***************************************************
*			Memory Mapp Function		*
&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&************/
void *Mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
	void *rt;
	rt = mmap(addr, len, prot, flags, fd, offset);
	if(rt == MAP_FAILED)				// MAP_FAILED = ((void *)-1) = 0xFFFFFFFF
		unix_error("Mmap error");
	return rt;
}

void Munmap(void *start, size_t length)
{
	int rt = munmap(start, length);
	if(rt == -1)
		unix_error("Munmap() error");
}


/***************************************************
	*Wrappers for Standard I/O functions
***************************************************/

FILE *Fopen(const char *filename, const char *mode)
{
	FILE *fp = fopen(filename, mode);
	if( NULL == fp)
		unix_error("Fopen error");
	return fp;
}

char *Fgets(char *ptr, int n, FILE *stream)
{
	char *ret = NULL;
	if (((ret = fgets(ptr, n, stream)) == NULL) && ferror(stream))
		app_error("Fgets error");
	return ret;
}

void Fputs(const char *ptr, FILE *stream)
{
	if(fputs(ptr, stream) == EOF)
		unix_error("Fputs error");
}

/*************************************************************
	* Sockets interface wrappers	*
**************************************************************/
int Socket(int domain, int type, int protocol)
{
	int rc;
	if((rc = socket(domain, type, protocol)) < 0)
		unix_error("Socket error");
	return rc;
}

void Setsockopt(int s, int level, int optname, const void *optval, int optlen)
{
	int rc;
	if((rc = setsockopt(s, level, optname, optval, optlen)) < 0)
		unix_error("Setsockopt error");
}

void Bind(int sockfd, struct sockaddr *my_addr, int addrlen)
{
	int rc;
	if((rc = bind(sockfd, my_addr, addrlen)) < 0)
		unix_error("Bind error");
}

int Accept(int listenfd, struct sockaddr *addr, int* addrlen)
{
	int rt;
	rt = accept(listenfd, addr, addrlen);
	if (rt == -1)
	{
		unix_error("Accept error");
	}
	return rt;
}

/********************************************************
	*The Rio package robust I/O functions
**********************************************************/

/* rio_readn robustly read n bytes(unbuffered)*/
ssize_t rio_readn(int fd, void * userbuf, size_t n)
{
	size_t nleft = n;
	ssize_t nread;
	char *bufp = userbuf;
	while( nleft > 0)
	{
		if ((nread = read(fd , bufp, nleft)) < 0 )
		{
			if(errno == EINTR)
				nread = 0;
			else 
				nread = -1;
		}
		else if (nread == 0)
		{
			break;
		}
		nleft -= nread;
		bufp += nread;
	}
	return (n - nleft);
}

ssize_t Rio_readn(int fd, void *userbuf, size_t n)
{
	ssize_t rt;
	rt = rio_readn(fd, userbuf, n);
	if (rt == -1)
		unix_error("Rio_readn error");
	return rt;
}

/*	robustly write n bytes (unbuffered)	*/
ssize_t rio_writen(int fd, void *userbuf, size_t n)
{
	size_t nleft = n;
	ssize_t nwritten;
	char *bufp = userbuf;
	while( nleft >0 )
	{
		if((nwritten = write(fd, bufp, nleft)) <= 0)
		{
			if(errno == EINTR)
				nwritten = 0;
			else 
				return  -1;
		}
		nleft -= nwritten;
		bufp += nwritten;
	}
	return n;
}

ssize_t Rio_writen(int fd, void *userbuf, size_t n)
{
	ssize_t rt;
	rt = rio_writen(fd, userbuf, n);
	if (rt == -1)
		unix_error("Rio_writen error");
	return rt;
}



/*	Associate a descriptor with a read buffer and reset buffer*/
void rio_readinitb(rio_t *rp, int fd)
{
	rp->rio_fd = fd;
	rp->rio_cnt = 0;
	rp->rio_bufptr = rp->rio_buf;
}

void Rio_readinitb(rio_t *rp, int fd)
{
	rio_readinitb(rp, fd);
}

/*
 * rio_read - This is a wrapper for the Unix read() function that
 * transfers min(n,rio_cnt) bytes from an internal buffer to a user
 * buffer, where n is the number of bytes requested by the user and 
 * rio_cnt is the number if unread bytes in the internal buffer, On
 * entry, rio_read() refills the internal buffer via a call to
 * read() if the internal buffer is empty
*/
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n)
{
	int cnt;

	while(rp->rio_cnt <= 0)			/*refill if buf is empty*/
	{		
		rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
		if( rp->rio_cnt < 0)
		{
			if(errno != EINTR)			// interrupted by sig handler return	
				return -1;
		}
		else if(rp->rio_cnt == 0)		//EOF
			return 0;
		else
			rp->rio_bufptr = rp->rio_buf;	//reset buffer ptr
	}

	/*Copy min(n, rp->rio_cnt) bytes from internal buf to user buf*/
	cnt = n;
	if(rp->rio_cnt < n)
		cnt = rp->rio_cnt;
	//memcpy((void *)usrbuf, (const void *)rp->rio_bufptr, (size_t)cnt);
	memcpy(usrbuf, rp->rio_bufptr, cnt);
	rp->rio_bufptr += cnt;
	rp->rio_cnt -= cnt;
	return cnt;

}

static ssize_t Rio_read(rio_t *rp, char *usrbuf, size_t n)
{
	ssize_t rt;
	rt = rio_read(rp, usrbuf, n);
	if (rt == -1)
		unix_error("Rio_read error");
	return rt;
}

/*	robustly read a text line(buffered)	*/
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
	int n, rc;
	char c, *bufp = usrbuf;
	for ( n = 1; n < maxlen; n++)
	{
		if((rc = rio_read(rp, &c, 1)) == 1)
		{
			*bufp++ = c;
			if( c =='\n')
				break;
		}
		else if(rc == 0)
		{
			if(n == 1)
				return 0;
			else 
				break;
		}else
			return -1;
	}
	*bufp = 0;
	return n;
}

ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
	ssize_t rt;
	rt = rio_readlineb(rp, usrbuf, maxlen);
	if (rt == -1)
		unix_error("Rio_readlineb error");
	return rt;
}

/*	Robustly read n bytes(buffered)	*/
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n)
{
	int rc, nleft;
	char *bufp = usrbuf;
	nleft = n;
	while( nleft > 0)
	{
		if ((rc = rio_read(rp, bufp, nleft)) < 0)
		{
			if(errno != EINTR)
				return -1;
			else 
				continue;
		}
		else if (rc == 0)
		{
			return 0;
		}
		else
		{
			bufp += rc;
			nleft -= rc;
		}
	}
	return n - nleft;
}

ssize_t Rio_readnb(rio_t *rp, void *usrbuf, size_t n)
{
	ssize_t rt;
	rt = rio_readnb(rp, usrbuf, n);
	if (rt == -1)
		unix_error("Rio_readnb error");
	return rt;
}

/***************************************************
	*	Client/server helper functions	*
****************************************************/
//Èô³É¹¦·µ»ØÃèÊö·û£¬Èôunix³ö´í·µ»Ø-1£¬ÈôDNS³ö´í·µ»Ø-2
int open_clientfd(char *hostname, int port)
{
	int clientfd, conn;
	struct hostent *hp;
	struct sockaddr_in serv_in;
	struct in_addr ip;

	clientfd = socket(AF_INET, SOCK_STREAM, 0);
	if(clientfd < 0)
		return -1;
	
	hp = gethostbyname(hostname);
	if(hp == NULL)
		return -2;
	bzero((char*)&serv_in, sizeof(serv_in));
	bcopy((char*)hp->h_addr, (char*)&serv_in.sin_addr.s_addr, hp->h_length);
	serv_in.sin_family = AF_INET;
	serv_in.sin_port = htons(port);
	if( connect(clientfd, (struct sockaddr *)&serv_in, sizeof(serv_in)) < 0)
		return -1;
	else
		return clientfd;
}

int Open_clientfd(char *hostname, int port)
{
	int rt;
	rt = open_clientfd(hostname, port);
	if ( rt == -1 )
		unix_error("Open_clientfd unix error");
	if ( rt == -2)
		unix_error("Open_clientfd DNS error");
	return rt;
}

int open_listenfd(int port)
{
	int listenfd, optval = 1;
	struct sockaddr_in serveraddr;

	/**Create a socket descriptor*/
	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
		return -1;

	/*Eliminates "Address already in use" error from bind*/
	if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(int)) < 0)
		return -1;

	/**Listenfd will be an endpoint for all requests to port on any ip address for this host*/
	bzero((char*)&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons((unsigned short)port);

	if(bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
		return -1;

	/*Make it a listening socket ready to accept connection request*/
	if(listen(listenfd, LISTENQ) < 0)
		return -1;
	return listenfd;
}

int Open_listenfd(int port)
{
	int rt;
	rt = open_listenfd(port);
	if( rt == -1)
		unix_error("open_listenfd error");
	return rt;
}

/****************************************************
	*DNS interface wrappers*
*****************************************************/
struct hostent *Gethostbyname(const char *hostname)
{
	struct hostent *hp;
	hp = gethostbyname(hostname);
	if (hp == NULL)
	{
		dns_error("Gethostbyname error");
	}
	return hp;
}

struct hostent *Gethostbyaddr(const char *addr, int len, int type)
{
	struct hostent *hp;
	hp = gethostbyaddr(addr, len, type);
	if (hp == NULL)
	{
		dns_error("Gethostbyaddr error");
	}
	return hp;
}


/*********************************************************/
/*Wrappers for Pthreads thread control functions*/
/*********************************************************/
void Pthread_create(pthread_t *tid, const pthread_attr_t *attr, void * (*routine)(void *), void *arp)
{
	int rt; 
	rt = pthread_create(tid, attr, routine, arp);
	if(rt != 0)
		 posix_error(rt, "Pthread_create error");
		 
}

void Pthread_cancel(pthread_t tid)
{
	int rt = pthread_cancel(tid);
	if(rt != 0)
		posix_error(rt, "Pthread_cancel error");
}

void Pthread_join(pthread_t tid, void **thread_return)
{
	int rt = pthread_join(tid, thread_return);
	if( rt != 0 )
		posix_error(rt, "pthread_join error");
}

void Pthread_detach(pthread_t tid)
{
	int rt = pthread_detach(tid);
	if( rt != 0)
		posix_error(rt, "pthread_detach error");
}

void Pthread_exit(void *retval)
{
	pthread_exit(retval);
}

pthread_t Pthread_self(void)
{
	pthread_self();
}

void Pthread_once(pthread_once_t *once_control, void (*init_function)(void))
{
	pthread_once(once_control, init_function);
}

/**********************************************************
	*	Wrappers for Posix semaphore	*
***********************************************************/
void Sem_init(sem_t *sem, int n, unsigned int value)
{
	int rt = sem_init(sem, n, value);
	if(rt == -1)
		unix_error("P error");
}

void P(sem_t *s)
{
	int rt = sem_wait(s);
	if(rt == -1)
		unix_error("P error");
}

void V(sem_t *s)
{
	int rt = sem_post(s);
	if(rt == -1)
		unix_error;
}

/******************************************************/
/*Wrappers for dynamic storage allocation functions*/
/*****************************************************/
void *Malloc(size_t size)
{
	void *ptr;
	ptr = malloc(size);
	if( NULL == ptr)
		unix_error("Malloc error");
	return ptr;
}

void *Calloc(size_t number, size_t size)
{
	void *ptr;
	if( NULL == (ptr = calloc(number, size)))
		unix_error("Malloc error");
	return ptr;
}

void Free(void *ptr)
{
	free(ptr);
}


void pr_exit(int status)
{

	if(WIFEXITED(status))
		printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
	else if(WIFSIGNALED(status))
		printf("abnormal termination, exit status = %d%s\n", WTERMSIG(status), WCOREDUMP(status)?" (core file generated)" : "");
	else 	if(WIFSTOPPED(status))
		printf("child stopped, signal number = %d\n", WSTOPSIG(status));

}



/***********************************************
				ourhdr.c
*************************************************/

static void err_doit(int , const char *, va_list);

/*Nonfatal error related to a system call
 print a message and return*/
void err_ret(const  char *fmt, ...)
{
	va_list	ap;
	va_start(ap, fmt);
	err_doit(1, fmt, ap);
	va_end(ap);
	return;
}

/*Fatal error related to a system call*/
void err_sys(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	err_doit(1, fmt, ap);
	va_end(ap);
	exit(1);
}

/*fatal error related to a system call.
 print a mesg, dump core , and terminate*/
void err_dump(const char *fmt, ...)
{
	va_list		ap;
	va_start(ap, fmt);
	err_doit(1, fmt, ap);
	va_end(ap);
	abort();		/*dump core and terminate*/
	exit(1);
}

/*non fatal error unrelated to a system call
print a msg and return*/
void err_msg(const char *fmt, ...)
{
	va_list		ap;
	va_start(ap, fmt);
	err_doit(0, fmt, ap);
	va_end(ap);
	return;
}

/* Fatal error unrelated to a system call
print a msg and terminate*/
void err_quit(const char * fmt, ...)
{
	va_list		ap;
	va_start(ap, fmt);
	err_doit(0, fmt, ap);
	va_end(ap);
	exit(1);
}

/*Print a message and return to caller
 * Caller specifies "errnoflag"*/
 static void err_doit(int errflag, const char * fmt, va_list ap)
 {
	 int	errno_save;
	 char	buf[MAXLINE];
	 errno_save = errno;	/*value caller might want printed*/
	 vsprintf(buf, fmt, ap);
	 if(errflag)
		 sprintf(buf + strlen(buf), ": %s", strerror(errno_save));
	strcat(buf, "\n");
	fflush(stdout);			/*in case stdout and stderr are the same*/
	fputs(buf, stderr);
	fflush(NULL);		/*flushes all stdio output stream*/
	return;
 }

#ifdef PATH_MAX
static int pathmax = PATH_MAX;
#else
static int pathmax = 0;
#endif

#define PATH_MAX_GUESS	1024
 char *path_alloc(int *size)
 {
	 char *ptr;
	 if (pathmax == 0)
	 {
		 errno = 0;
		 if ((pathmax = pathconf("/", _PC_PATH_MAX)) < 0)
		 {
			 if(errno == 0)
				 pathmax = PATH_MAX_GUESS;
			 else
				 err_sys("pathconf error for _PC_PATH_MAX");
		 }
		 else pathmax++;	/*add one since it's relative to root*/
	 }
	 if((ptr = malloc(pathmax + 1)) == NULL)
		 err_sys("malloc error for pathname");
	 if(size != NULL)
		 *size = pathmax + 1;
	 return(ptr);
 }


/*************************************************************************
 *						Advance I/O										*
 ************************************************************************/

void set_fl(int fd, int flags)	/* flags are file status flags to turn on */
{
	int		val;
	if((val = fcntl(fd, F_GETFL, 0)) < 0 )
		err_sys("fcntl F_GETFL error");
	val |= flags;			/* turn on flags */

	if(fcntl(fd, F_SETFL, val) < 0)
		err_sys("fctnl F_SETFL, val");

}

void clr_fl(int fd, int flags)	/* flags are file status flags to turn on*/
{
	int		val;
	if((val = fcntl(fd, F_GETFL, 0)) < 0)
		err_sys("fcntl F_GETFL error");
	val &= ~flags;

	if(fcntl(fd, F_SETFL, val) < 0)
		err_sys("fctnl F_SETFL, val");
}



int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
									/* cmd: F_GETLK, F_SETLK, F_SETLKW*/
	struct flock	lock;
	lock.l_type		= type;			/* F_RDLCK, F_WRLCK, F_UNLCK*/
	lock.l_start	= offset;		/* byte offset, relative to l_whence	*/
	lock.l_whence	= whence;		/* SEEK_SET. SEEK_CUR, SEEK_END*/
	lock.l_len		= len;			/* bytes(0 means to EOF)*/

	return	fcntl(fd, cmd, &lock);
}


/****************Print Proc Mask Signal*******************/
void pr_mask(const char *str)
{
	sigset_t		sigset;
	int				errno_save;

	errno_save = errno;
	Sigprocmask(0, NULL, &sigset);
	
	printf("%s", str);
	if(sigismember(&sigset, SIGINT))	printf("SIGINT ");
	sigaddset(&sigset, SIGQUIT);
	if(sigismember(&sigset, SIGQUIT))	printf("SIGQUIT ");
	if(sigismember(&sigset, SIGUSR1))	printf("SIGUSR1 ");
	if(sigismember(&sigset, SIGALRM))	printf("SIGALRM ");

	printf("\n");
	errno = errno_save;
}


/*********************************************************************/
/*					Synchronous Function implement by signal	*/
/**********************************************************************/
/*
static volatile sig_atomic_t	sigflag = 0;	// set nonzero by signal handler

static	sigset_t				newmask, oldmask, zeromask;

static void sig_usr(int signo)	//one signal handler for SIGUSR1 and SIGUSR2
{
	sigflag = 1;
	return;
}

void TELL_WAIT(void)
{
	Signal(SIGUSR1, sig_usr);
//	Signal(SIGUSR2, sig_usr);

	sigemptyset(&zeromask);
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGUSR1);
//	sigaddset(&newmask, SIGUSR2); 
			// block sigusr1 and sigusr2, and save current signal mask
	Sigprocmask(SIG_BLOCK, &newmask, &oldmask);
}

void TELL_PARENT(pid_t pid)
{
	kill(pid, SIGUSR1);				// tell parent we're done
}

void WAIT_PARENT(void)
{
	while(sigflag == 0)
		sigsuspend(&zeromask);		// and wait for parent
	sigflag = 0;	//reset signal mask to original value 
	Sigprocmask(SIG_SETMASK, &oldmask, NULL);
}

void TELL_CHILD(pid_t pid)
{
	kill(pid, SIGUSR1);
}

void WAIT_CHILD(void)
{
	while(sigflag == 0)
		sigsuspend(&zeromask);		// wait for child

	sigflag = 0;		//reset signal mask to original value 
	Sigprocmask(SIG_SETMASK, &oldmask, NULL);
}
*/

static	int	pfd1[2], pfd2[2];

void TELL_WAIT(void)
{
	Pipe(pfd1);
	Pipe(pfd2);
}


void TELL_PARENT(pid_t pid)
{
	if(write(pfd2[1], "c", 1) != 1)
		unix_error("write error");
}

void WAIT_PARENT(void)
{
	char c;
	if(read(pfd1[0], &c, 1) != 1)
		unix_error("read error");
	if(c != 'p')
		unix_error("WAIT_PARENT:incorrect data");

}


void TELL_CHILD(pid_t pid)
{
	if(write(pfd1[1], "p", 1) != 1)
		unix_error("write error");
}

void WAIT_CHILD(void)
{
	char c;
	if(read(pfd2[0], &c, 1) != 1)
		unix_error("read error");
	if(c != 'c')
		unix_error("WAIT_CHILD:incorrect data");
}

int s_pipe(int fd[2])
{
	return socketpair(AF_UNIX, SOCK_STREAM, 0, fd);
}


extern int	debug;		/*caller must define and set this: nonzero if interactive, zero if demo*/

/*initialize syslog(), if running as daemon*/
void log_open(const char *ident, int option, int facility)
{
	if(debug == 0)
		openlog(ident, option, facility);
}

/*Print a message and return to caller. Caller specifies "errnoflag" and "priority" */
void log_doit(int errnoflag, int priority, const char *fmt, va_list ap)
{
	int		errno_save;
	char	buf[MAXLINE];
	errno_save = errno;			//value caller might want printed
	vsprintf(buf, fmt, ap);
	if(errnoflag)
		sprintf(buf + strlen(buf), ": %s", strerror(errno_save));
	strcat(buf, "\n");
	if (debug)
	{
		fflush(stdout);
		fputs(buf, stderr);
		fflush(stderr);
	}else
		syslog(priority, buf);
	return;
}

/*Nonfatal error related to a system call. Print a message with the system's errno value and return */
void log_ret(const char *fmt, ...)
{
	va_list		ap;
	va_start(ap, fmt);
	log_doit(1, LOG_ERR, fmt, ap);
	va_end(ap);
	return;
}

/*Fatal error related to a system call. Print a message and terminate*/
void log_sys(const char *fmt, ...)
{
	va_list		ap;
	va_start(ap, fmt);
	log_doit(1, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(2);
}

/*Nonfatal error related to a system call. Print a message and return*/
void log_msg(const char *fmt, ...)
{
	va_list		ap;
	va_start(ap, fmt);
	log_doit(0, LOG_ERR, fmt, ap);
	va_end(ap);
	return;
}


/*Fatal error unrelated to a system call. Print a message and terminate*/
void log_quit(const char *fmt, ...)
{
	va_list		ap;
	va_start(ap, fmt);
	log_doit(0, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(2);
}


/****************************END*****************************/

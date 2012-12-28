#ifndef __CSAPP_H__
#define __CSAPP_H__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <setjmp.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/times.h>
#include <syslog.h>

#include <limits.h>
#include <stdarg.h>
#include <dirent.h>

/*Misc constants*/
#define BUFFSIZE 8192
#define RIO_BUFSIZE 8192
#define MAXLINE 8192
#define LISTENQ 1024

// Persistent state for the  robust I/O package
typedef struct 
{
	int rio_fd;
	int rio_cnt;  // unread bytes in internal buf
	char *rio_bufptr;
	char rio_buf[RIO_BUFSIZE];
}rio_t;


typedef struct sockaddr SA;
extern char **environ;

/*Our own error-handling functions*/
void unix_error(char *msg);
void posix_error(int code, char *msg);
void dns_error(char *msg);
void app_error(char *msg);


/*Process control wrapper*/
pid_t Fork(void);
void Execve(const char*filename, char *const argv[], char *const envp[]);
pid_t Wait(int *status);
pid_t Waitpid(pid_t pid, int *iptr, int options);
void Kill(pid_t pid, int signum);
unsigned int Sleep(unsigned int seconds);
void Pause(void);
unsigned int Alarm(unsigned int seconds);
void Setpgid(pid_t pid, pid_t pgid);
pid_t Getpgrp();
int System(char *);
clock_t Times(struct tms *);		/* This interface is obsoleted by getrusage(2) and gettimeofday(2)*/
long Sysconf(int);
char *Getenv(const char *name);

/*Communication between Process*/
void Pipe(int fildes[2]);
FILE *Popen(const char* cmdstring, const char* type);
void Pclose(FILE *fp);

/*Signal wrapper*/
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);
void Sigfillset(sigset_t *set);
void Sigaddset(sigset_t *set, int signum);
void Sigdelset(sigset_t *set, int signum);
void Sigemptyset(sigset_t *set);
void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
int Sigismember(const sigset_t *set, int signum);

/*Unix I/O wrappers*/
int Open(const char *pathname, int flags, mode_t mode);
ssize_t Read(int fd, void *buf, size_t count);
ssize_t Write(int fd, const void *buf, size_t count);
off_t Lseek(int fildes, off_t offset, int whence);
void Close(int fd);
int Select(int n,fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
int Dup2(int oldd, int newd);
void Stat(const char *filename, struct stat *buf);
void Fstat(int fd, struct stat *buf);

/*Memory mapping wrappers*/
void *Mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
void Munmap(void *start, size_t length);

/*Standard I/O wrappers*/
void Fclose(FILE *fp);
FILE *Fdopen(int fd, const char *type);
char *Fgets(char *ptr, int n, FILE *stream);
FILE *Fopen(const char *filename, const char *mode);
void Fputs(const char *ptr, FILE *stream);
size_t Fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
void Fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

/*Dynamic storage allocation wrappers*/
void *Malloc(size_t size);
void *Realloc(void *ptr, size_t size);
void *Calloc(size_t number, size_t size);
void Free(void *ptr);

/* Sockets interface wrappers */
int Socket(int domain, int type, int protocol);
void Setsockopt(int s, int level, int optname, const void *optval, int optlen);
void Bind(int sockfd, struct sockaddr *my_addr, int addlen);
void Listen(int s, int backlog);
int Accept(int listenfd, struct sockaddr *addr, int *addrlen);
void Connect(int sockfd, struct sockaddr *serv_addr, int addrlen);

// Rio (Robust I/O) package
ssize_t rio_writen(int fd, void *buf, size_t n);
ssize_t rio_readn(int fd, void *buf, size_t n);
void rio_readinitb(rio_t *rp, int fd);
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n);


/** Wrappers for Rio package*/
ssize_t Rio_writen(int fd, void *buf, size_t n);
ssize_t Rio_readn(int fd, void *buf, size_t n);
void Rio_readinitb(rio_t *rp, int fd);
ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);
ssize_t Rio_readnb(rio_t *rp, void *usrbuf, size_t n);


/***********************netlink***************************/
struct hostent *Gethostbyname(const char *hostname);
struct hostent *Gethostbyaddr(const char *addr, int len, int type);

//���ɹ���������������unix������-1����DNS������-2
int open_clientfd(char *hostname, int port);
int open_listenfd(int port);

/*******Wrappers for client/server helper function**********/
int Open_clientfd(char *hostname, int port);
int Open_listenfd(int port);


/*Pthreads thread control wrappers */
void Pthread_create(pthread_t *tid, const pthread_attr_t *attr, void *(*routine)(void *), void *arp);
void Pthread_cancel(pthread_t tid);
void Pthread_join(pthread_t tid, void **thread_return);
void Pthread_detach(pthread_t tid);
void Pthread_exit(void *retval);
pthread_t Pthread_self(void);
void Pthread_once(pthread_once_t *once_control, void (*init_function)(void));

/* POSIX semaphore wrappers */
void Sem_init(sem_t *sem, int pshared, unsigned int value);
void P(sem_t *s);
void V(sem_t *s);

void pr_exit(int status);


// ourhdr.h
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define DIR_MODE (FILE_MODE | S_IXUSR | S_ISGRP | S_IXOTH)

void err_ret(const  char *fmt, ...);
void err_sys(const char *fmt, ...);
void err_quit(const char * msg, ...);
void err_dump(const char *fmt, ...);
void err_msg(const char *fmt, ...);


char *path_alloc(int *size);


#define	read_lock(fd, offset, whence, len)		lock_reg(fd, F_SETLK, F_RDLCK, offset, whence, len)
#define	readw_lock(fd, offset, whence, len)		lock_reg(fd, F_SETLKW, F_RDLCK, offset, whence, len)
#define	write_lock(fd, offset, whence, len)		lock_reg(fd, F_SETLK, F_WRLCK, offset, whence, len)
#define	writew_lock(fd, offset, whence, len)	lock_reg(fd, F_SETLKW, F_WRLCK, offset, whence, len)
#define	un_lock(fd, offset, whence, len)		lock_reg(fd, F_SETLK, F_UNLCK, offset, whence, len)

int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len);

void set_fl(int fd, int flags);	/* flags are file status flags to turn on */
void clr_fl(int fd, int flags);	/* flags are file status flags to turn on*/

void pr_mask(const char *str);
void TELL_WAIT(void);
void TELL_PARENT(pid_t pid);
void WAIT_PARENT(void);
void TELL_CHILD(pid_t pid);
void WAIT_CHILD(void);

int s_pipe(int fd[2]);
/*Used when we had planned to send an fd using send_fd() but encountered an error instead, We send
	the error back using the send_fd()/ recv_fd() protocol */
//int send_err(int clifd, int errcode, const char *msg);
/*Pass a file descriptor to another process. If fd< 0 , then -fd is sent back instead as the error status*/
//int send_fd(int clifd, int fd);


void log_msg(const char *,...);
void log_open(const char *, int, int);
void log_quit(const char *, ...);
void log_ret(const char *, ...);
void log_sys(const char *, ...);
#endif

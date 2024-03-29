#include "csapp.h"
#include	<sys/uio.h>
#include	<stddef.h>

#define	CONTROLLEN	(sizeof(struct cmsghdr) +sizeof(int))	/*size of control buffer to send/recv one file descriptor */

static struct cmsghdr	*cmptr = NULL;		// buffer is malloc'ed first time

/*Pass a file descriptor to another process. If fd< 0 , then -fd is sent back instead as the error status*/
/*
Description:
	The function will packaging and send fd to clifd.
	
Input
	clifd	: destination
	fd		: the file descriptor will be send
Return
	Failed	: -1
	Success	: 0
*/
int send_fd(int clifd, int fd)
{
	struct iovec	iov[1];
	struct msghdr	msg;
	char			buf[2];	//send_fd/ recv_fd() 2-byte protocol

	iov[0].iov_base = buf;
	iov[0].iov_len	= 2;
	msg.msg_iov		= iov;
	msg.msg_iovlen	= 1;
	msg.msg_name	= NULL;
	msg.msg_namelen	= 0;
	if (fd < 0)
	{
		msg.msg_control	= NULL;
		msg.msg_controllen	= 0;
		buf[1] = -fd;		// nonzero status means error
		/*if(buf[1] == 0)
			buf[1] = 1;*/
	}else
	{
		if(cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL)
			return -1;
		cmptr->cmsg_level	= SOL_SOCKET;
		cmptr->cmsg_type	= SCM_RIGHTS;
		cmptr->cmsg_len		= CONTROLLEN;
		msg.msg_control		= (caddr_t)cmptr;
		msg.msg_controllen	= CONTROLLEN;
		*(int *)CMSG_DATA(cmptr) = fd;		/* the fd to pass*/
		buf[1] = 0;		// zero status means OK
	}
	buf[0] = 0;			// null byte flag to recv_fd
	if(sendmsg(clifd, &msg, 0) != 2)
		return -1;

	return 0;
}

/*Used when we had planned to send an fd using send_fd() but encountered an error instead, We send
	the error back using the send_fd()/ recv_fd() protocol */
int send_err(int clifd, int errcode, const char *msg)
{
	int		n;
	if((n = strlen(msg)) > 0)
		if(Rio_writen(clifd, (void *)msg, n) != n)		//send the error message
			return -1;
	if(errcode >= 0)
		errcode = -1;		// must be negative
	if(send_fd(clifd, errcode) < 0)
		return -1;

	return 0;
}

/*Receive a file descriptor from another process(a server). In addtion, any data received from the server is passed 
	to (*userfunc)(STDERR_FILENO, buf, nbytes). We have a 2-byte protocol for recerving the fd from send_fd()*/
/*
Return
	Success	: newfd
	Failed	: -1
*/
int recv_fd(int servfd, ssize_t (*userfunc)(int, const void *, size_t))
{
	int				newfd, nread, status;
	char			*ptr, buf[MAXLINE];
	struct iovec	iov[1];
	struct msghdr	msg;
	status	= -1;
	for (; ; )
	{
		iov[0].iov_base	= buf;
		iov[0].iov_len	= sizeof(buf);
		msg.msg_iov		= iov;
		msg.msg_iovlen	= 1;
		msg.msg_name	= NULL;
		msg.msg_namelen	= 0;
		if(cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL )
			return -1;
		msg.msg_control	= (caddr_t)cmptr;
		msg.msg_controllen = CONTROLLEN;
		if((nread = recvmsg(servfd, &msg, 0)) < 0)
			err_sys("recvmsg error");
		else if (nread == 0)
		{
			err_ret("connecting closed by server");
			return -1;
		}
		/* see if this it the final data with null & status . NUll must be next to last byte of buffer, status
		byte is last byte, Zero status means there must be a file descriptor to receive.*/
		//buf[nread] = '\0';
		//printf("recv:%d %s\n",nread, buf);
		for (ptr = buf; ptr < &buf[nread]; )
		{
			if(*ptr++ == 0)
			{
				if(ptr != &buf[nread - 1])
					err_dump("message format error");
				status = *ptr & 255;
				if (status == 0)
				{
					if(msg.msg_controllen != CONTROLLEN)
						err_dump("status = 0 but no fd");
					newfd = *(int *)CMSG_DATA(cmptr);			//new descriptor
				}else
					newfd = -status;
				nread -= 2;
			}
		}
		if(nread > 0)
			if((*userfunc)(STDERR_FILENO, buf, nread) != nread)		// write buf to stdout
				return -1;
		if(status >= 0)		//final data has arrived
			return newfd;
	}
}




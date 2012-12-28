#include	"csapp.h"
#include	<syslog.h>
#define		CL_OPEN	"open"			//client's request for server
#define		CS_OPEN "/home/stevens/opend"		//server's well-known name

extern int	debug	;	/*nonzero if interactive( not daemon)*/
extern char	errmsg[];	//error message string to return to client
extern int	oflag;		//open() flag: O_xxxx ....
extern char	*pathname;	// of file to open() for client

int	cli_args(int, char **);
void request(char *, int, int); 

int serv_listen(const char *name);
int	serv_accept(int listenfd, uid_t *uidptr);
int cli_conn(const char *name);

typedef struct //one client struct per connected client
{
	int		fd;	//fd, or -1 if available
	uid_t	uid;
}Client;

/*both manipulated by client_XXX() functions */
extern Client	*client;		//ptr to malloc'ed array
extern int		client_size;	//entries in client[] array

int		client_add(int, uid_t);
void	client_del(int);
void	loop(void);
void	request_sock(char *, int, int, uid_t);

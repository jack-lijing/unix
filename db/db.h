#include "csapp.h"
#include <ndbm.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stddef.h>
#include "csapp.h"

#define	DB_INSERT	1
#define	DB_REPLACE	2


typedef struct 
{
	int		idxfd;		/*fd for index file*/
	int		datfd;		/*fd for data file*/
	int		oflag;		/*flags for open()/db_open():O_xxx*/

	char	*idxbuf;	/*malloc'ed buffer for index record	*/
	char	*datbuf;
	char	*name;		// name db waas opened under
	off_t	idxoff;		//offset in index file of index record
						//	actual key is at(idxoff + PTR_SZ + IDXLEN_SZ)
	size_t	idxlen;		//length of index record
						// excludes IDXLEN_SZ bytes at front of index record, includes newline ata end of index record
	off_t	datoff		//offset in data file of data record
	size_t	datlen		//length of data record, includes newline at end

	off_t	ptrval		// contents of chain ptr in index record
	off_t	ptroff		// offset of chain ptr tha points to this index record

};
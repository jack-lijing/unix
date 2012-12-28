#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <dirent.h>

#define MAXLINE	4096

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define DIR_MODE (FILE_MODE | S_IXUSR | S_ISGRP | S_IXOTH)

void err_ret(const  char *fmt, ...);
void err_sys(const char *fmt, ...);
void err_quit(const char * msg, ...);
void err_dump(const char *fmt, ...);
void err_msg(const char *fmt, ...);


char *path_alloc(int *size);



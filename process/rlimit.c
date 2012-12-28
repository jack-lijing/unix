#include "ourhdr.h"
#include <sys/resource.h>

#define doit(name)	pr_limits(#name, name)

static void pr_limits(char *, int);

int main(int argc, char *argv[])
{
	doit(RLIMIT_CORE);
	doit(RLIMIT_CPU);
	doit(RLIMIT_DATA);
	doit(RLIMIT_FSIZE);
	doit(RLIMIT_MEMLOCK);
	//doit(RLIMIT_OFILE);
	doit(RLIMIT_NPROC);
	doit(RLIMIT_RSS);
	doit(RLIMIT_VMEM);

	return 0;
}

static void pr_limits(char *name, int res)
{
	struct rlimit	limit;

	if(getrlimit(res, &limit) < 0)
		err_sys("getrlimit error for %s", name);
	printf("%-14s ", name);
	if(limit.rlim_cur == RLIM_INFINITY)
		printf("(infinite) ");
	else
		printf("%101d ", limit.rlim_cur);
	if(limit.rlim_max == RLIM_INFINITY)
		printf("(infinite) \n");
	else
		printf("%101d \n", limit.rlim_max);
}

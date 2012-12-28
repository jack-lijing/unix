#include "csapp.h"

static void pr_times(clock_t, struct tms *, struct tms *);
static void do_cmd(char *);

int main(int argc, char *argv[])
{
	int	i;
	for (i =1; i< argc; i++)
	{
		do_cmd(argv[i]);	
	}
	return 0;
}

static void do_cmd(char *cmd)
{
	struct tms	tmsstart, tmsend;
	clock_t		start, end;
	int			status;

	fprintf(stderr, "\ncommand: %s\n", cmd);
	start	=	Times(&tmsstart);
	status	=	System(cmd);
	end		=	Times(&tmsend);

	pr_times(end-start, &tmsstart, &tmsend);
	pr_exit(status);
}

static void pr_times(clock_t real, struct tms *tmsstart, struct tms *tmsend)
{
	static long		clktck	=	0;
	if(clktck == 0)
		clktck = Sysconf(_SC_CLK_TCK);

	fprintf(stderr, " real:  %7.2f\n", real / (double)clktck);
	fprintf(stderr, " user:  %7.2f\n", (tmsend->tms_utime - tmsstart->tms_utime) / (double)clktck);
	fprintf(stderr, " sys:  %7.2f\n", (tmsend->tms_stime - tmsstart->tms_stime) / (double)clktck);
	fprintf(stderr, " child user:  %7.2f\n", (tmsend->tms_cutime - tmsstart->tms_cutime) / (double)clktck);
	fprintf(stderr, " child sys:  %7.2f\n", (tmsend->tms_cstime - tmsstart->tms_cstime) / (double)clktck);
}


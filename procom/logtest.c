#include <syslog.h>

int main(int argc, char *argv[])
{
	openlog("logtest", LOG_PID, LOG_MAIL);
	syslog(LOG_ERR, "log error");
	syslog(LOG_WARNING, "log warning");
	syslog(LOG_INFO, "log info");
	
	printf("Hello, world\n");
	

	return 0;
}

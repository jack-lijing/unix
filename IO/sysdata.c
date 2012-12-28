#include "ourhdr.h"
#include <pwd.h>

struct passwd * getpwname(const char *name)
{
	struct passwd *ptr;

	setpwent();
	while ((ptr = getpwent()) != NULL)
	{
		if(strcmp(name, ptr->pw_name) == 0)
			break;		//find a match
	}
	endpwent();
	return ptr;
}
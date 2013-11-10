#include <unistd.h>
#include <sys/types.h>

#include "thinklight.h"

extern int light_fd;

#ifdef PROC_LEGACY
#define ON_MESSAGE "on"
#else
#define ON_MESSAGE "255"
#endif

void tl_on(void) {
	if(light_fd < 0)
		return;
	
	lseek(light_fd, 0, SEEK_SET);
	write(light_fd, ON_MESSAGE, sizeof(ON_MESSAGE) - 1);
}

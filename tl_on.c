#include <unistd.h>
#include <sys/types.h>

#include "thinklight.h"

extern int light_fd;

void tl_on(void) {
	if(light_fd < 0)
		return;
	
	lseek(light_fd, 0, SEEK_SET);
	write(light_fd, "on", sizeof("on") - 1);
}

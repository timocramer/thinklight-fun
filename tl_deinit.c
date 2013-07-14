#include <unistd.h>

#include "thinklight.h"

extern int light_fd;

void tl_deinit(void) {
	if(light_fd < 0)
		return;
	
	close(light_fd);
	light_fd = -1;
}

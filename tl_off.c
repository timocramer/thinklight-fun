#include <unistd.h>
#include <sys/types.h>

#include "thinklight.h"

extern int light_fd;

#ifdef PROC_LEGACY
#define OFF_MESSAGE "off"
#else
#define OFF_MESSAGE "0"
#endif

void tl_off(void) {
	if(light_fd < 0)
		return;
	
	lseek(light_fd, 0, SEEK_SET);
	write(light_fd, OFF_MESSAGE, sizeof(OFF_MESSAGE) - 1);
}

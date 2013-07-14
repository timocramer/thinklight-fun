#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#include "thinklight.h"

extern int light_fd;

int tl_status(void) {
	char buf[sizeof("status:\t\tof")];
	char n_or_f;
	
	if(light_fd < 0)
		return TL_UNDEF;
	
	lseek(light_fd, 0, SEEK_SET);
	
	read(light_fd, buf, sizeof(buf) - 1);
	buf[sizeof(buf) - 1] = 0;
	
	/* we only look if the char which should be
	after the 'o' is an 'n' or an 'f'. */
	n_or_f = buf[strlen("status:\t\to")];
	if(n_or_f == 'n')
		return TL_ON;
	else if(n_or_f == 'f')
		return TL_OFF;
	else
		return TL_UNDEF;
}

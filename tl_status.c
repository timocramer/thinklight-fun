#include <unistd.h>
#include <sys/types.h>

#include "thinklight.h"

extern int light_fd;

int tl_status(void) {
	char buf[sizeof("status:\t\tof") - 1];
	char n_or_f;
	
	if(light_fd < 0)
		return TL_UNDEF;
	
	lseek(light_fd, 0, SEEK_SET);
	
	if(read(light_fd, buf, sizeof(buf)) != sizeof(buf))
		return TL_UNDEF;
	
	/* we only look if the char which should be
	after the 'o' is an 'n' or an 'f'. */
	n_or_f = buf[sizeof(buf) - 1];
	if(n_or_f == 'n')
		return TL_ON;
	else if(n_or_f == 'f')
		return TL_OFF;
	else
		return TL_UNDEF;
}

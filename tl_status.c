#include <unistd.h>
#include <sys/types.h>

#include "thinklight.h"

extern int light_fd;

int tl_status(void) {
#ifdef PROC_LEGACY
	char buf[sizeof("status:\t\tof") - 1];
	char n_or_f;
#else
	char buf[sizeof("0")];
#endif
	
	if(light_fd < 0)
		return TL_UNDEF;
	
	lseek(light_fd, 0, SEEK_SET);
	
	if(read(light_fd, buf, sizeof(buf)) != sizeof(buf))
		return TL_UNDEF;
	
#ifdef PROC_LEGACY
	/* we only look if the char which should be
	after the 'o' is an 'n' or an 'f'. */
	n_or_f = buf[sizeof(buf) - 1];
	if(n_or_f == 'n')
		return TL_ON;
	else if(n_or_f == 'f')
		return TL_OFF;
	else
		return TL_UNDEF;
#else
	/* we look for 0, which means off or something else, which means on */
	if(buf[0] == '0')
		return TL_OFF;
	else
		return TL_ON;
#endif
}

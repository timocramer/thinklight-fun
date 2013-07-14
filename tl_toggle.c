#include "thinklight.h"

extern int light_fd;

void tl_toggle(void) {
	int status = tl_status();
	
	if(status == TL_OFF)
		tl_on();
	else if(status == TL_ON)
		tl_off();
}

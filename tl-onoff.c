#include <stdio.h>

#include "thinklight.h"

int main(void) {
	if(tl_init()) {
		fputs("Light Interface cannot be opened!\n", stderr);
		return 1;
	}
	
#ifdef ON
	tl_on();
#elif defined OFF
	tl_off();
#endif
	
	tl_deinit();
	return 0;
}

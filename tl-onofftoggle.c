#include <unistd.h>

#include "thinklight.h"

#define ERROR_MESSAGE "Light Interface cannot be opened!\n"

int main(void) {
	if(tl_init()) {
		write(2, ERROR_MESSAGE, sizeof(ERROR_MESSAGE) - 1);
		return 1;
	}
	
#ifdef ON
	tl_on();
#elif defined OFF
	tl_off();
#elif defined TOGGLE
	tl_toggle();
#endif
	
	tl_deinit();
	return 0;
}

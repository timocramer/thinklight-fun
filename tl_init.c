#include <sys/stat.h>
#include <fcntl.h>

#include "thinklight.h"

#ifdef PROC_LEGACY
#define LIGHT_PATH "/proc/acpi/ibm/light"
#else
#define LIGHT_PATH "/sys/class/leds/tpacpi::thinklight/brightness"
#endif

int light_fd = -1;

int tl_init(void) {
	light_fd = open(LIGHT_PATH, O_RDWR);
	if(light_fd < 0)
		return 1;
	return 0;
}

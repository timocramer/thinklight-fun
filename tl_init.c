#include <sys/stat.h>
#include <fcntl.h>

#include "thinklight.h"

#define LIGHT_PATH "/proc/acpi/ibm/light"

int light_fd = -1;

int tl_init(void) {
	light_fd = open(LIGHT_PATH, O_RDWR);
	if(light_fd < 0)
		return 1;
	return 0;
}
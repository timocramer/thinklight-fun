#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "thinklight.h"

#define LIGHT_PATH "/proc/acpi/ibm/light"

static int light_fd = -1;

int tl_init(void) {
	light_fd = open(LIGHT_PATH, O_RDWR);
	if(light_fd < 0)
		return 1;
	return 0;
}

void tl_deinit(void) {
	if(light_fd < 0)
		return;
	
	close(light_fd);
	light_fd = -1;
}

void tl_on(void) {
	if(light_fd < 0)
		return;
	
	lseek(light_fd, 0, SEEK_SET);
	write(light_fd, "on", strlen("on"));
}

void tl_off(void) {
	if(light_fd < 0)
		return;
	
	lseek(light_fd, 0, SEEK_SET);
	write(light_fd, "off", strlen("off"));
}

void tl_toggle(void) {
	int status = tl_status();
	
	if(status == TL_OFF)
		tl_on();
	else if(status == TL_ON)
		tl_off();
}

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

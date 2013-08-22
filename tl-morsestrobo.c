#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "thinklight.h"

#ifdef MORSE
static const char* morse_code(char x) {
	if(x >= 'a' && x <= 'z')
		x &= ~0x20;
	
	switch(x) {
	case '\'': return ".----.";
	case '(': return "-.--.";
	case ')': return "-.--.-";
	case '+': return ".-.-.";
	case ',': return "--..--";
	case '-': return "-....-";
	case '.': return ".-.-.-";
	case '/': return "-..-.";
	case '0': return "-----";
	case '1': return ".----";
	case '2': return "..---";
	case '3': return "...--";
	case '4': return "....-";
	case '5': return ".....";
	case '6': return "-....";
	case '7': return "--...";
	case '8': return "---..";
	case '9': return "----.";
	case ':': return "---...";
	case ';': return "-.-.-.";
	case '=': return "-...-";
	case '?': return "..--..";
	case '@': return ".--.-.";
	case 'A': return ".-";
	case 'B': return "-...";
	case 'C': return "-.-.";
	case 'D': return "-..";
	case 'E': return ".";
	case 'F': return "..-.";
	case 'G': return "--.";
	case 'H': return "....";
	case 'I': return "..";
	case 'J': return ".---";
	case 'K': return "-.-";
	case 'L': return ".-..";
	case 'M': return "--";
	case 'N': return "-.";
	case 'O': return "---";
	case 'P': return ".--.";
	case 'Q': return "--.-";
	case 'R': return ".-.";
	case 'S': return "...";
	case 'T': return "-";
	case 'U': return "..-";
	case 'V': return "...-";
	case 'W': return ".--";
	case 'X': return "-..-";
	case 'Y': return "-.--";
	case 'Z': return "--..";
	case '_': return "..--.-";
	}
	return NULL;
}

#define DIT 170000
#define DAH (DIT * 3)

#define AFTER_SYMBOL DIT
#define AFTER_LETTER (DIT * 2)
#define AFTER_WORD (DIT * 4)

static void morse_char(char c) {
	const char *p = morse_code(c);
	
	if(p != NULL) {
		while(*p != 0) {
			tl_toggle();
			if(*p == '.')
				usleep(DIT);
			else
				usleep(DAH);
			
			tl_toggle();
			usleep(AFTER_SYMBOL);
			
			++p;
		}
	}
	else if(c == ' ')
		usleep(AFTER_WORD);
}

static void morse_string(char *str) {
	char *c = str;
	
	while(*c != 0) {
		morse_char(*c);
		++c;
		if(*c != 0)
			usleep(AFTER_LETTER);
	}
}
#endif

#define LOCKFILE "/tmp/thinklight.lock"

static int lock(void) {
	int fd;
	
	/* if the file exists, fail. otherwise, create it */
	fd = open(LOCKFILE, O_CREAT | O_EXCL | O_WRONLY, 0644);
	if(fd < 0)
		return 1;
	
	/* close it again, we only want the file to exist */
	close(fd);
	
	return 0;
}

static void unlock(void) {
	if(unlink(LOCKFILE) == -1)
		perror("unlink " LOCKFILE);
}

static int initial_status = TL_OFF;

static void restore_status_and_exit(int s) {
	(void) s;
	
	if(initial_status == TL_ON)
		tl_on();
	else
		tl_off();
	
	tl_deinit();
	unlock();
	exit(0);
}

static void install_signal_handler(void) {
	struct sigaction sa;
	
	sa.sa_handler = restore_status_and_exit;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if((sigaction(SIGINT, &sa, NULL) == -1)
	|| (sigaction(SIGTERM, &sa, NULL) == -1))
		fputs("Cannot install signal handler! "
				"The initial light-status will not be saved!\n", stderr);
}

#ifdef STROBO
#define DELAY 40000
#endif

int main(int argc, char **argv) {
	if(tl_init()) {
		fputs("Light Interface cannot be opened!\n", stderr);
		return 1;
	}
	if(tl_status() == TL_ON)
		initial_status = TL_ON;
	
	if(lock()) {
		if(errno != EEXIST)
			perror("open " LOCKFILE);
		else
			fputs("Thinklight is in use!\n", stderr);
		return 1;
	}
	
	install_signal_handler();
	
#ifdef MORSE
	if(argc < 2) {
		fputs(argv[0], stderr);
		fputs(": No arguments!\n", stderr);
		
		tl_deinit();
		unlock();
		return 1;
	}
	
	/* morse the arguments */
	int i = 1;
	while(1) {
		morse_string(argv[i]);
		
		++i;
		
		if(i >= argc) /* is it the last argument? */
			break;
		morse_char(' ');
	}
	
	/* save the initial status */
	if(initial_status == TL_ON)
		tl_on();
	else
		tl_off();
	
	tl_deinit();
	unlock();
#elif defined STROBO
	/* use argc and argv so the compiler doesn't complain */
	(void) argc;
	(void) argv;
	
	while(1) {
		tl_toggle();
		usleep(DELAY);
	}
	/* deinit and unlock happens in the signal handler */
#endif
	
	return 0;
}

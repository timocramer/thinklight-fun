#ifndef THINKLIGHT_H
#define THINKLIGHT_H

/*
opens the thinklight-file in /proc
you have to call tl_init before you can use the other functions in this file
returns 1 on success, 0 otherwise
*/
int tl_init(void);
/* closes the thinklight-file */
void tl_deinit(void);

/* turns the thinklight on or off */
void tl_on(void);
void tl_off(void);
void tl_toggle(void);

/*
returns the status of the thinklight
TL_UNDEF should never happen
*/
#define TL_ON 1
#define TL_OFF 0
#define TL_UNDEF (-1)
int tl_status(void);

#endif

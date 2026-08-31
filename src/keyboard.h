#ifndef COS_KEYBOARD_H
#define COS_KEYBOARD_H
#include <stdint.h>

void keyboard_init(void);
int keyboard_has_key(void);
char keyboard_getchar(void);
void keyboard_poll(void);

#endif

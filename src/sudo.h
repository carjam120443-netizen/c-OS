#ifndef COS_SUDO_H
#define COS_SUDO_H
#include <stdint.h>

int sudo_authorized(uint32_t uid);
int sudo_execute(uint32_t uid, const char *command);

#endif

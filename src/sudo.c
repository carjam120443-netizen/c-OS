#include "sudo.h"
#include "users.h"

int sudo_authorized(uint32_t uid) {
    return users_is_admin(uid) ? 1 : 0;
}

int sudo_execute(uint32_t uid, const char *command) {
    /* Command dispatch will be connected to the c-OS shell later. */
    if (!command || !sudo_authorized(uid)) return -1;
    return 0;
}

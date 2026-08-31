#include "sudo.h"
#include "users.h"

static uint32_t command_has_prefix(const char *line, const char *prefix) {
    uint32_t i = 0;
    while (prefix[i]) {
        if (line[i] != prefix[i]) return 0;
        i++;
    }
    return line[i] == ' ' || line[i] == '\0';
}

int sudo_authorized(uint32_t uid) {
    return users_is_admin(uid) ? 1 : 0;
}

int sudo_execute(uint32_t uid, const char *command) {
    if (!command || !command[0]) return -2;
    if (!sudo_authorized(uid)) return -1;

    if (command_has_prefix(command, "su")) {
        if (users_switch_to_root() != 0) return -4;
        return 1;
    }

    if (command_has_prefix(command, "whoami") ||
        command_has_prefix(command, "pwd") ||
        command_has_prefix(command, "help") ||
        command_has_prefix(command, "clear") ||
        command_has_prefix(command, "polling-sys-start")) return 0;

    return -3;
}

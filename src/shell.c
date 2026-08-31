#include "shell.h"
#include "users.h"
#include "sudo.h"

static uint32_t str_eq(const char *a, const char *b) {
    uint32_t i = 0;
    while (a[i] && b[i] && a[i] == b[i]) i++;
    return a[i] == '\0' && b[i] == '\0';
}

void shell_init(void) { }
void shell_print_prompt(void) { }

void shell_run_command(const char *line) {
    if (!line || !line[0]) return;

    /* Built-in command dispatcher. */
    if (str_eq(line, "polling-sys-start")) {
        /* The kernel's main loop already polls the PS/2 keyboard continuously. */
        return;
    }
    if (str_eq(line, "whoami")) {
        (void)users_current_uid();
        return;
    }
    if (str_eq(line, "sudo")) {
        (void)sudo_execute(users_current_uid(), "");
        return;
    }
    if (str_eq(line, "help") || str_eq(line, "clear") || str_eq(line, "pwd")) return;
}

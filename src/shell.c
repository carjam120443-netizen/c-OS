#include "shell.h"
#include "users.h"
#include "sudo.h"

static uint32_t str_eq(const char *a, const char *b) {
    uint32_t i = 0;
    while (a[i] && b[i] && a[i] == b[i]) i++;
    return a[i] == '\0' && b[i] == '\0';
}

static const char *skip_spaces(const char *s) {
    while (*s == ' ') s++;
    return s;
}

void shell_init(void) { }
void shell_print_prompt(void) { }

void shell_run_command(const char *line) {
    if (!line || !line[0]) return;
    line = skip_spaces(line);

    if (str_eq(line, "polling-sys-start")) return;
    if (str_eq(line, "whoami")) { (void)users_current_uid(); return; }
    if (str_eq(line, "help") || str_eq(line, "clear") || str_eq(line, "pwd")) return;

    if (line[0] == 's' && line[1] == 'u' && line[2] == 'd' && line[3] == 'o' && line[4] == ' ') {
        const char *command = skip_spaces(line + 5);
        int result = sudo_execute(users_current_uid(), command);
        (void)result;
        return;
    }
}

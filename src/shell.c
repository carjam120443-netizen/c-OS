#include "shell.h"
#include "users.h"
#include "sudo.h"
#include "pkg.h"

static uint32_t str_eq(const char *a, const char *b) {
    uint32_t i = 0;
    while (a[i] && b[i] && a[i] == b[i]) i++;
    return a[i] == '\0' && b[i] == '\0';
}

static const char *skip_spaces(const char *s) { while (*s == ' ') s++; return s; }

static int starts_with(const char *s, const char *p) {
    uint32_t i = 0; while (p[i]) { if (s[i] != p[i]) return 0; i++; } return s[i] == ' ' || s[i] == 0;
}

void shell_init(void) { pkg_init(); }
void shell_print_prompt(void) { }

void shell_run_command(const char *line) {
    if (!line || !line[0]) return;
    line = skip_spaces(line);

    if (str_eq(line, "polling-sys-start") || str_eq(line, "whoami") ||
        str_eq(line, "help") || str_eq(line, "clear") || str_eq(line, "pwd")) return;
    if (str_eq(line, "su")) return;

    if (starts_with(line, "pkg")) {
        const char *args = skip_spaces(line + 3);
        (void)pkg_command(args);
        return;
    }

    if (starts_with(line, "sudo")) {
        const char *command = skip_spaces(line + 4);
        (void)sudo_execute(users_current_uid(), command);
        return;
    }
}

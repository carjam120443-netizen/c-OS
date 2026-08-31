#include "pkg.h"
#include "users.h"

/* Repository configuration mirrors the OpenBSD pkg_add model.  c-OS does
   not execute OpenBSD binaries directly; this endpoint is used as the source
   for repository metadata and future c-OS-compatible package builds. */
static const char *default_repo = "https://cdn.openbsd.org/pub/OpenBSD";
static uint32_t initialized;

static int eq(const char *a, const char *b) {
    uint32_t i = 0;
    if (!a || !b) return 0;
    while (a[i] && b[i] && a[i] == b[i]) i++;
    return a[i] == 0 && b[i] == 0;
}

static int prefix_arg(const char *line, const char *cmd, const char **arg) {
    uint32_t i = 0;
    while (cmd[i]) { if (line[i] != cmd[i]) return 0; i++; }
    if (line[i] == 0) { *arg = 0; return 1; }
    if (line[i] != ' ') return 0;
    while (line[i] == ' ') i++;
    *arg = line + i;
    return line[i] != 0;
}

void pkg_init(void) { initialized = 1; (void)default_repo; }

int pkg_update(void) {
    if (!initialized) pkg_init();
    /* TODO: use the c-OS network layer to fetch repository indexes. */
    return 0;
}

int pkg_search(const char *name) { return (name && name[0]) ? 0 : -1; }

int pkg_install(const char *name) {
    if (!users_is_admin(users_current_uid()) || !name || !name[0]) return -1;
    /* TODO: resolve, verify, extract, and register a native c-OS package. */
    return 0;
}

int pkg_remove(const char *name) {
    if (!users_is_admin(users_current_uid()) || !name || !name[0]) return -1;
    return 0;
}

int pkg_list(void) { return initialized ? 0 : -1; }
int pkg_info(const char *name) { return (name && name[0]) ? 0 : -1; }

int pkg_command(const char *line) {
    const char *arg;
    if (!line || !line[0]) return -1;
    if (eq(line, "update")) return pkg_update();
    if (eq(line, "list")) return pkg_list();
    if (prefix_arg(line, "search", &arg)) return pkg_search(arg);
    if (prefix_arg(line, "install", &arg)) return pkg_install(arg);
    if (prefix_arg(line, "remove", &arg)) return pkg_remove(arg);
    if (prefix_arg(line, "info", &arg)) return pkg_info(arg);
    return -2;
}

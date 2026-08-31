#include "pkg.h"
#include "users.h"

/* c-OS pkg is OpenBSD-repository inspired.  The native c-OS package format,
   signatures, indexes, and installation backend can be filled in as the
   filesystem and networking layers mature.  The repository endpoint is kept
   configurable instead of pretending that OpenBSD binaries are c-OS-native. */
static const char *default_repo = "https://cdn.openbsd.org/pub/OpenBSD";
static uint32_t initialized;

static int eq(const char *a, const char *b) {
    uint32_t i = 0;
    if (!a || !b) return 0;
    while (a[i] && b[i] && a[i] == b[i]) i++;
    return a[i] == 0 && b[i] == 0;
}

void pkg_init(void) { initialized = 1; (void)default_repo; }

int pkg_update(void) {
    if (!initialized) pkg_init();
    /* Repository download/index parsing will be connected to networking. */
    return 0;
}

int pkg_search(const char *name) {
    if (!name || !name[0]) return -1;
    return 0;
}

int pkg_install(const char *name) {
    if (!users_is_admin(users_current_uid()) || !name || !name[0]) return -1;
    /* Package verification/extraction belongs here once native package I/O exists. */
    return 0;
}

int pkg_remove(const char *name) {
    if (!users_is_admin(users_current_uid()) || !name || !name[0]) return -1;
    return 0;
}

int pkg_list(void) { return initialized ? 0 : -1; }
int pkg_info(const char *name) { return (name && name[0]) ? 0 : -1; }

int pkg_command(const char *line) {
    if (!line || !line[0]) return -1;
    if (eq(line, "update")) return pkg_update();
    if (eq(line, "list")) return pkg_list();
    return -2;
}

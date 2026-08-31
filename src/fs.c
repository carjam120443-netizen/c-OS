#include "fs.h"

/*
 * Initial in-memory root filesystem.
 * This is the first c-OS filesystem implementation: a tiny read-only RAM FS
 * compiled into the kernel. A disk-backed filesystem can replace it later.
 */
static const fs_node_t rootfs[] = {
    { "/", "", 1 },
    { "/bin", "", 1 },
    { "/boot", "", 1 },
    { "/dev", "", 1 },
    { "/etc", "", 1 },
    { "/home", "", 1 },
    { "/lib", "", 1 },
    { "/proc", "", 1 },
    { "/root", "", 1 },
    { "/tmp", "", 1 },
    { "/usr", "", 1 },
    { "/usr/bin", "", 1 },
    { "/usr/lib", "", 1 },
    { "/usr/share", "", 1 },
    { "/var", "", 1 },
    { "/var/db", "", 1 },
    { "/var/db/pkg", "", 1 },
    { "/var/log", "", 1 },

    { "/etc/os-release",
      "NAME=c-OS\nPRETTY_NAME=c-OS\nID=cos\nVERSION=0.1-dev\n", 0 },

    { "/etc/pkg.conf",
      "PKG_DB=/var/db/pkg\nPKG_REPO=/usr/share/pkg/repository\n", 0 }
};

static uint8_t mounted = 0;

void fs_mount_root(void) {
    mounted = 1;
}

const fs_node_t *fs_root(void) {
    return mounted ? rootfs : 0;
}

uint32_t fs_node_count(void) {
    return mounted ? (uint32_t)(sizeof(rootfs) / sizeof(rootfs[0])) : 0;
}

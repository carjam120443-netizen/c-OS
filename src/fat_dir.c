#include "fat_dir.h"
#include "fat_disk.h"

/* Directory enumeration is exposed separately so the shell/VFS can consume
 * filesystem entries without knowing FAT on-disk structures. */
int fat_readdir_root(fat_dirent_t *entries, uint32_t capacity, uint32_t *count) {
    (void)entries; (void)capacity;
    if (count) *count = 0;
    return 0;
}
int fat_lookup(const char *path, fat_dirent_t *entry) {
    (void)path; (void)entry;
    return 0;
}

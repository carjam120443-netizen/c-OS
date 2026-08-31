#ifndef COS_FAT_DIR_H
#define COS_FAT_DIR_H
#include <stdint.h>
typedef struct { char name[13]; uint8_t directory; uint32_t size; uint32_t cluster; } fat_dirent_t;
int fat_readdir_root(fat_dirent_t *entries, uint32_t capacity, uint32_t *count);
int fat_lookup(const char *path, fat_dirent_t *entry);
#endif

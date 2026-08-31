#ifndef COS_FAT_DISK_H
#define COS_FAT_DISK_H
#include <stdint.h>
int fat_disk_mount(uint32_t start_lba);
int fat_disk_read_file(const char *path, uint8_t *buffer, uint32_t capacity, uint32_t *size);
#endif

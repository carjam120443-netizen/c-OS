#ifndef COS_FAT_WRITE_H
#define COS_FAT_WRITE_H
#include <stdint.h>
int fat_create_file(const char *path, const uint8_t *data, uint32_t size);
int fat_mkdir(const char *path);
#endif

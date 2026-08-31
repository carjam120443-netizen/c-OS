#ifndef COS_FAT_H
#define COS_FAT_H

#include <stdint.h>

/* Initial FAT filesystem interface.
 * FAT12/16/32 parsing and block-device support will be expanded later. */

typedef enum {
    FAT_NONE = 0,
    FAT12,
    FAT16,
    FAT32
} fat_type_t;

typedef struct {
    fat_type_t type;
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
} fat_volume_t;

int fat_mount(const void *boot_sector, fat_volume_t *volume);
const char *fat_type_name(fat_type_t type);

#endif

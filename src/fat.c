#include "fat.h"

static uint16_t read16(const uint8_t *p) {
    return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}

int fat_mount(const void *boot_sector, fat_volume_t *volume) {
    const uint8_t *b = (const uint8_t *)boot_sector;

    if (!boot_sector || !volume)
        return 0;

    /* Validate the common FAT boot-sector signature. */
    if (b[510] != 0x55 || b[511] != 0xAA)
        return 0;

    volume->bytes_per_sector = read16(&b[11]);
    volume->sectors_per_cluster = b[13];

    if (volume->bytes_per_sector == 0 || volume->sectors_per_cluster == 0)
        return 0;

    /* FAT type detection will use cluster counts once a block device exists.
       Until then, this is an initial FAT volume interface. */
    volume->type = FAT_NONE;
    return 1;
}

const char *fat_type_name(fat_type_t type) {
    switch (type) {
        case FAT12: return "FAT12";
        case FAT16: return "FAT16";
        case FAT32: return "FAT32";
        default:    return "FAT";
    }
}

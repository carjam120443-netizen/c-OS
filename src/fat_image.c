#include <stdint.h>
#include "block.h"

/* Simple RAM-backed disk image adapter. Useful for early filesystem tests. */
static const uint8_t *image;
static uint32_t image_sectors;

void fat_image_register(const void *data, uint32_t sectors) {
    image=(const uint8_t*)data; image_sectors=sectors;
}

int fat_image_read(uint32_t lba, uint8_t *buffer) {
    if(!image || !buffer || lba>=image_sectors) return 0;
    for(uint32_t i=0;i<BLOCK_SECTOR_SIZE;i++) buffer[i]=image[lba*BLOCK_SECTOR_SIZE+i];
    return 1;
}

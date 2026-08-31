#ifndef COS_IDE_H
#define COS_IDE_H
#include <stdint.h>
int ide_init(void);
int ide_read_sector(uint32_t lba, uint8_t *buffer);
uint32_t ide_sector_count(void);
#endif

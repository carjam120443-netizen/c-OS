#ifndef COS_FAT12_H
#define COS_FAT12_H
#include <stdint.h>
uint16_t fat12_entry(const uint8_t *fat, uint32_t cluster);
#endif

#ifndef COS_BLOCK_H
#define COS_BLOCK_H
#include <stdint.h>
#define BLOCK_SECTOR_SIZE 512u
typedef int (*block_read_fn)(uint32_t lba, uint8_t *buffer);
int block_register(block_read_fn reader, uint32_t sectors);
int block_read(uint32_t lba, uint8_t *buffer);
uint32_t block_sector_count(void);
#endif

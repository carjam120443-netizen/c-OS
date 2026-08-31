#include "block.h"
static block_read_fn reader;
static uint32_t sectors;
int block_register(block_read_fn fn, uint32_t count) { if (!fn || !count) return 0; reader=fn; sectors=count; return 1; }
int block_read(uint32_t lba, uint8_t *buffer) { if (!reader || !buffer || lba>=sectors) return 0; return reader(lba,buffer); }
uint32_t block_sector_count(void) { return sectors; }

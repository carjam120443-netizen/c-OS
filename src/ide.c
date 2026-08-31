#include "ide.h"

/* Hardware PIO implementation is intentionally isolated here. VirtualBox can
 * later expose an IDE disk through these routines without changing FAT/VFS. */
int ide_init(void) { return 0; }
int ide_read_sector(uint32_t lba, uint8_t *buffer) { (void)lba; (void)buffer; return 0; }
uint32_t ide_sector_count(void) { return 0; }

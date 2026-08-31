#include "fat_write.h"

/* Safe placeholder boundary for future FAT mutation. Writes must be added
 * together with free-cluster allocation, FAT mirroring, directory entry
 * creation, and crash-safe metadata updates. */
int fat_create_file(const char *path, const uint8_t *data, uint32_t size) {
    (void)path; (void)data; (void)size; return 0;
}
int fat_mkdir(const char *path) { (void)path; return 0; }

#ifndef COS_FS_H
#define COS_FS_H

#include <stdint.h>

typedef struct {
    const char *path;
    const char *contents;
    uint8_t is_dir;
} fs_node_t;

void fs_mount_root(void);
const fs_node_t *fs_root(void);
uint32_t fs_node_count(void);

#endif

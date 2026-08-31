#include "fat12.h"
uint16_t fat12_entry(const uint8_t *fat, uint32_t cluster) {
    uint32_t off = cluster + cluster / 2;
    uint16_t v = (uint16_t)fat[off] | ((uint16_t)fat[off + 1] << 8);
    return (cluster & 1) ? (v >> 4) & 0x0fff : v & 0x0fff;
}

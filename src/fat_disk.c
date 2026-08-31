#include "fat_disk.h"
#include "block.h"
#include <stddef.h>
#include <string.h>

static uint32_t base_lba;
static uint16_t bytes_per_sector;
static uint8_t sectors_per_cluster;
static uint32_t fat_start, data_start;
static uint32_t root_cluster;
static uint8_t mounted;
static uint8_t sector[BLOCK_SECTOR_SIZE];

static uint16_t r16(const uint8_t *p){return (uint16_t)p[0]|((uint16_t)p[1]<<8);}
static uint32_t r32(const uint8_t *p){return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}

int fat_disk_mount(uint32_t start_lba){
    if(!block_read(start_lba,sector)) return 0;
    if(sector[510]!=0x55 || sector[511]!=0xaa) return 0;
    bytes_per_sector=r16(&sector[11]); sectors_per_cluster=sector[13];
    if(bytes_per_sector!=512 || !sectors_per_cluster) return 0;
    fat_start=start_lba+r16(&sector[14]);
    data_start=fat_start+(uint32_t)sector[16]*r16(&sector[22]);
    root_cluster=r32(&sector[44]);
    base_lba=start_lba; mounted=1; return 1;
}

int fat_disk_read_file(const char *path,uint8_t *buffer,uint32_t capacity,uint32_t *size){
    (void)path; (void)buffer; (void)capacity;
    if(size) *size=0;
    /* Directory walking and FAT cluster-chain reading are the next layer. */
    return mounted ? 0 : 0;
}

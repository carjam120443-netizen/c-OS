#include "fat_disk.h"
#include "block.h"
#include <stddef.h>

static uint32_t base_lba, fat_start, data_start, root_cluster;
static uint16_t bytes_per_sector, reserved_sectors, root_entries;
static uint8_t sectors_per_cluster, fat_count;
static uint32_t sectors_per_fat;
static uint8_t mounted, fat32;
static uint8_t sector[BLOCK_SECTOR_SIZE];

static uint16_t r16(const uint8_t *p){return (uint16_t)p[0]|((uint16_t)p[1]<<8);}
static uint32_t r32(const uint8_t *p){return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static int eq(const char*a,const char*b){while(*a&&*b){if(*a++!=*b++)return 0;}return *a==0&&*b==0;}
static uint32_t cluster_lba(uint32_t c){return data_start+(c-2u)*sectors_per_cluster;}
static uint32_t fat_entry(uint32_t c){
    uint32_t off=c*(fat32?4u:2u), lba=fat_start+off/bytes_per_sector, pos=off%bytes_per_sector;
    if(!block_read(lba,sector)) return 0;
    uint32_t v=fat32?r32(&sector[pos]):r16(&sector[pos]);
    return fat32?v&0x0fffffff:v;
}
static int eoc(uint32_t c){return fat32?c>=0x0ffffff8u:c>=0xfff8u;}

int fat_disk_mount(uint32_t start_lba){
    if(!block_read(start_lba,sector)||sector[510]!=0x55||sector[511]!=0xaa)return 0;
    bytes_per_sector=r16(&sector[11]); reserved_sectors=r16(&sector[14]); sectors_per_cluster=sector[13]; fat_count=sector[16];
    uint16_t fat16=r16(&sector[22]); uint32_t fat32size=r32(&sector[36]);
    root_entries=r16(&sector[17]);
    if(bytes_per_sector!=512||!sectors_per_cluster||!reserved_sectors||!fat_count)return 0;
    sectors_per_fat=fat16?fat16:fat32size; fat32=!fat16;
    fat_start=start_lba+reserved_sectors;
    uint32_t root_dir_sectors=((uint32_t)root_entries*32u+bytes_per_sector-1u)/bytes_per_sector;
    data_start=fat_start+(uint32_t)fat_count*sectors_per_fat+root_dir_sectors;
    root_cluster=fat32?r32(&sector[44]):0;
    if(!fat32) root_cluster=0;
    base_lba=start_lba; mounted=1; return 1;
}

/* Find a short 8.3 filename in the root directory and read its cluster chain. */
int fat_disk_read_file(const char *path,uint8_t *buffer,uint32_t capacity,uint32_t *size){
    if(size)*size=0; if(!mounted||!path||!buffer||!capacity)return 0;
    while(*path=='/')path++;
    if(!*path)return 0;
    uint32_t cluster=fat32?root_cluster:0;
    uint32_t root_lba;
    uint32_t root_sectors;
    if(fat32){root_lba=cluster_lba(cluster);root_sectors=sectors_per_cluster;}
    else {root_lba=base_lba+reserved_sectors+(uint32_t)fat_count*sectors_per_fat;root_sectors=((uint32_t)root_entries*32u)/bytes_per_sector;}
    for(uint32_t s=0;s<root_sectors;s++){
        if(!block_read(root_lba+s,sector))return 0;
        for(uint32_t off=0;off<512;off+=32){
            uint8_t first=sector[off]; if(first==0x00)return 0; if(first==0xe5||sector[off+11]==0x0f)continue;
            char name[13]; uint32_t n=0;
            for(int i=0;i<8&&sector[off+i]!=' ';i++)name[n++]=sector[off+i];
            if(sector[off+8]!=' '){name[n++]='.';for(int i=0;i<3&&sector[off+8+i]!=' ';i++)name[n++]=sector[off+8+i];}
            name[n]=0;
            if(!eq(name,path))continue;
            uint32_t c=(uint32_t)r16(&sector[off+26]); if(fat32)c|=r32(&sector[off+20])<<16;
            uint32_t remaining=r32(&sector[off+28]), written=0;
            while(c>=2&&!eoc(c)&&remaining){
                uint32_t count=sectors_per_cluster;
                for(uint32_t cs=0;cs<count&&remaining;cs++){
                    if(!block_read(cluster_lba(c)+cs,sector))return 0;
                    uint32_t take=remaining>512?512:remaining;
                    if(written+take>capacity)take=capacity-written;
                    for(uint32_t i=0;i<take;i++)buffer[written+i]=sector[i];
                    written+=take;remaining-=take;if(written>=capacity)remaining=0;
                }
                if(remaining)c=fat_entry(c); else break;
            }
            if(size)*size=written;return written>0||r32(&sector[off+28])==0;
        }
    }
    return 0;
}

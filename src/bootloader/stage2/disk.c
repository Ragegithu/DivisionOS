#include "disk.h"

#include "stdio.h"
#include "stdint.h"

static void lba_to_chs(uint32_t lba, uint16_t sectors_per_track, uint16_t heads, uint16_t* cylinder, uint16_t* head, uint16_t* sector)
{
    *sector   = (lba % sectors_per_track) +1;
    *head     = (lba / sectors_per_track) % heads;
    *cylinder = (lba / sectors_per_track) / heads;
}

extern bool _cdecl diskRead_asm(uint8_t sectors, uint16_t cylinder, uint8_t sector,
                         uint8_t head, uint8_t drive, void* buffer);

bool _cdecl diskRead(uint16_t drive, uint32_t lba, uint8_t sectors, void* buffer, uint16_t sectors_per_track, uint16_t heads)
{
    uint16_t cylinder, head, sector;
    lba_to_chs(lba,sectors_per_track, heads, &cylinder, &head, &sector);

    return diskRead_asm(sectors, cylinder,sector, head, drive,buffer);
}

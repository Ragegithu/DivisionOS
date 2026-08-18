#pragma once

#include "stdio.h"
#include "stdint.h"


static void lba_to_chs(uint32_t lba, uint16_t sectors_per_track, uint16_t heads, uint16_t* cylinder, uint16_t* head, uint16_t* sector);
bool _cdecl diskRead(uint16_t drive, uint32_t lba, uint8_t sectors, void* buffer, uint16_t sectors_per_track, uint16_t heads);

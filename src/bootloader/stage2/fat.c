#include "fat.h"

#define NULL 0

//floppy specific magic numbers;
static uint8_t g_Fat [9 * 512];
static DirectoryEntry g_RootDirectory [224];
static uint32_t g_RootDirectoryEnd;


int memcmp(const void* a, const void* b, uint32_t count) {
    const uint8_t* pa = a;
    const uint8_t* pb = b;
    while (count--) {
        if (*pa != *pb) return *pa - *pb;
        pa++; pb++;
    }
    return 0;
}


static BootSector g_BootSectorData;
BootSector* g_BootSector;

bool FAT_init(uint16_t drive)
{
    uint16_t ds_val;
    __asm mov ax, ds
    __asm mov [ds_val], ax
    
    bool ok = diskRead(drive, 0, 1, &g_BootSectorData, 18, 2);


uint8_t* raw = (uint8_t*)&g_BootSectorData;
    g_BootSector = &g_BootSectorData;  // now point the global at it

    // read FAT table — now g_BootSector fields are valid
    ok = diskRead(drive, g_BootSector->ReservedSectors, g_BootSector->SectorsPerFat,
                  g_Fat, g_BootSector->SectorsPerTrack, g_BootSector->Heads);
    
    if(!ok) {
    printf("failed");
    for(;;);
    }
    uint32_t lba = g_BootSector->ReservedSectors + g_BootSector->SectorsPerFat * g_BootSector->FatCount;
    uint32_t size = sizeof(DirectoryEntry) * g_BootSector->DirEntryCount;
    uint32_t sectors = (size / g_BootSector->BytesPerSector);

    if(size % g_BootSector->BytesPerSector > 0)
        sectors++;

    g_RootDirectoryEnd = lba + sectors;

    ok = ok && diskRead(drive, lba, sectors, g_RootDirectory,
                        g_BootSector->SectorsPerTrack, g_BootSector->Heads);

    if(!ok) {
    printf("FAT TABLE READ FAILED");
    for(;;);
    }
    return ok;
}


DirectoryEntry* FAT_find(const char* name)
{
    for(uint32_t i=0; i < g_BootSector->DirEntryCount; i++)
    {
        if(memcmp(name,g_RootDirectory[i].Name, 11) == 0)
        {
            return &g_RootDirectory[i];
        }
    }
    return NULL;
}
bool readFile(DirectoryEntry* fileEntry, uint16_t drive, uint8_t* outputBuffer)
{
    bool ok = true;
    uint16_t currentCluster = fileEntry->FirstClusterLow;

    do
    {
        uint32_t lba = g_RootDirectoryEnd + ( currentCluster - 2) * g_BootSector->SectorsPerCluster;
        ok = ok && diskRead(drive,lba, g_BootSector->SectorsPerCluster, outputBuffer,g_BootSector->SectorsPerTrack,g_BootSector->Heads);
        outputBuffer += g_BootSector->SectorsPerCluster * g_BootSector->BytesPerSector;

        uint32_t fatIndex = currentCluster *3 /2;
        if(currentCluster %2 == 0)
            currentCluster = (*(uint16_t*)(g_Fat + fatIndex)) & 0x0fff;
        else
            currentCluster = (*(uint16_t*)(g_Fat + fatIndex)) >> 4;

    }while(ok && currentCluster< 0x0ff8);

    return ok;
}
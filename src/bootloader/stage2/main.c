#include "stdint.h"
#include "stdio.h"
#include "fat.h"

#define NULL 0

void _cdecl cstart_(uint16_t bootDrive)
{
    printf("running stage 2 bootloader\r\n");
    

    //printf("DRIVE: ");
    //printf("%x", bootDrive);
    printf("calling fat_init\r\n");
    bool fatOk = FAT_init(bootDrive);



    printf("calling fat_find\r\n");
    DirectoryEntry* kernel = FAT_find("KERNEL  BIN");
    
    if(kernel == NULL)
    {
        printf("KERNEL NOT FOUND ");
        for(;;);
    }
    printf("found kernel!\r\n");

    printf("loading kernel\r\n");

    uint8_t* kernelBuffer = (uint8_t*)0xC000;
    bool ok = readFile(kernel, bootDrive, kernelBuffer);
    printf("Kernel readFile done ok");

    if(!ok)
    {
        printf("CANNOT READ KERNEL FILE ");
        for(;;);
    }
    
    return;
}
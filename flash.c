#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "configuration.h"

static void flash_create(void)
{
    uint8_t buf[1024];
    FILE * fp = fopen("flash.bin","w");
    
    memset(buf, 0xFF, sizeof(buf));

    for (int i=0; i < 64; i++)
    {
        fwrite(buf, 1, sizeof(buf), fp);
    }
    
    fclose(fp);
}

/**
 * @brief Write a full page to the flash device
 *
 * This function writes a full page (512 bytes) to the flash device.
 *
 * @param page The page number to write
 * @param data The data buffer to be written
 * @return 0 = ok , -1 = argumnet error
 */
int flash_write(unsigned int page, unsigned char * data)
{
    unsigned char buf[PAGE_SIZE];
    FILE * fp;
    
    if (page > (FLASH_SIZE / PAGE_SIZE) - 1)
    {
        return -1;
    }

    fp = fopen("flash.bin", "r+");

    if (!fp)
    {
        flash_create();
        fp = fopen("flash.bin","r+");
    }
    
    fseek(fp, page * PAGE_SIZE, SEEK_SET);
    fread(buf, 1, PAGE_SIZE, fp);

    for (int i=0; i < sizeof(buf); i++)
    {
        data[i] = data[i] & buf[i];
    }
    
    fseek(fp, page * PAGE_SIZE, SEEK_SET);
    fwrite(data, 1, PAGE_SIZE, fp);
    fclose(fp);
    return 0;
}

/**
 * @brief Read a full page from the flash device
 *
 * This function reads a full page (512 bytes) from the flash device.
 *
 * @param page The page number to read
 * @param data Pointer to the buffer to be filled with the data
 * @return 0 = ok , -1 = argumnet error
 */
int flash_read(unsigned int page, unsigned char * data)
{
    FILE * fp;
    
    if (page > (FLASH_SIZE / PAGE_SIZE) - 1)
    {
        return -1;
    }

    fp = fopen("flash.bin", "r");

    if (!fp) 
    {
        flash_create();
        fp = fopen("flash.bin","r");
    }
    
    fseek(fp, page * PAGE_SIZE, SEEK_SET);
    fread(data, 1, PAGE_SIZE, fp);    
    fclose(fp);
    return 0;
}

/**
 * @brief Erase a sector in the flash device
 *
 * This function erases a full sector (16K bytes) from the flash device. After a sector
 * is erased all its bits are set to logic 1
 *
 * @param sector The sector number to erase
 * @return 0 = ok , -1 = argumnet error
 */
int flash_erase(unsigned int sector)
{
    uint8_t buf[SECTOR_SIZE];
    FILE * fp;
    
    if (sector > (FLASH_SIZE / SECTOR_SIZE) - 1)
    {
        return -1;
    }

    fp = fopen("flash.bin", "r+");

    if (!fp)
    {
        flash_create();
        fp = fopen("flash.bin","r+");
    }
    
    fseek(fp, sector * SECTOR_SIZE, SEEK_SET);
    memset(buf, 0xFF, sizeof(buf));
    fwrite(buf, 1, sizeof(buf), fp);    
    fclose(fp);    
    return 0;
}

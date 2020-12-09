#pragma once

#define FLASH_SIZE   1024*1024     // 1 MB
#define SECTOR_SIZE     8*1024     // 8 kB
#define PAGE_SIZE          512     // 512 Bytes

#define LAST_SECTOR FLASH_SIZE/(SECTOR_SIZE)
#define LAST_PAGE   FLASH_SIZE/PAGE_SIZE

enum mode
{
    READ = 0,
    WRITE = 1,
    IDLE = 2
};

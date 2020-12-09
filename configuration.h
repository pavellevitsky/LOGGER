#define FLASH_SIZE   64*1024     // 64 kB
#define SECTOR_SIZE   8*1024     // 8 kB
#define PAGE_SIZE        512     // 512 Bytes

#define LAST_SECTOR FLASH_SIZE/(SECTOR_SIZE)
#define LAST_PAGE   FLASH_SIZE/PAGE_SIZE

enum mode
{
    IDLE  = 0,
    READ  = 1,
    WRITE = 2
};

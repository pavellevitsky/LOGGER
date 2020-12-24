#define FLASH_SIZE   16*1024     // 64 kB
#define SECTOR_SIZE   2*1024     // 8 kB
#define PAGE_SIZE        512     // 512 Bytes

#define LAST_SECTOR FLASH_SIZE/(SECTOR_SIZE)
#define LAST_PAGE   FLASH_SIZE/PAGE_SIZE

enum mode
{
    IDLE  = 0,
    READ  = 1,
    WRITE = 2
};

int g_write_pointer;  // global var points to next write operation address
int g_read_pointer;   // global var points to next read operation address
int g_total_size;     // global var contains data size currently logged in flash

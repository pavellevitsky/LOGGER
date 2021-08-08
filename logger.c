#include <string.h>
#include <stdint.h>
#include "flash.h"
#include "configuration.h"

enum mode e_mode;

// declaration of static functios; documentation and implementation below.
static void copy_sector_to_local_buffer (uint16_t, unsigned char*);
static uint32_t address_to_page_number (uint32_t);
static uint16_t page_to_sector_number (uint32_t);

void logger_init (void)
{
    // initialise all global variables
    e_mode = IDLE;  
    g_write_pointer = 0;
    g_read_pointer = 0;
    g_total_size = 0;
}

int logger_open (int mode)
{
    if ((mode != READ) && (mode != WRITE))  // unexpected mode
    {
        return -1;
    }
    if (e_mode != IDLE)  // nesting is not allowed
    {
        return -2;
    }

    e_mode = mode;  // keep requested mode a global variable
    return 0;
}

void logger_close(void)
{
    e_mode = IDLE;
}

int logger_read (unsigned char *data, unsigned int *size)
{
    unsigned char buffer[SECTOR_SIZE];
    uint32_t page_first;
    uint32_t page_last;
    uint16_t sector_first;
    uint16_t sector_last;
    uint16_t b_read_pointer;
    uint16_t local_size;
    uint16_t size_remainder;

    if (*size == 0)
    {
        return -1;  // invalid data size
    }
    if (g_total_size == 0)
    {
        return -2;  // flash memory is empty - nothing to read
    }
    if (g_total_size < (int)*size)
    {
        *size = g_total_size;  // limit read data size by available data size
    }

    size_remainder = *size;  // how many data is left to read - is updated each section loop iteration

    page_first = address_to_page_number (g_read_pointer);              // get starting page of logged data
    page_last = address_to_page_number (g_read_pointer + *size - 1);   // get last page of logged data

    sector_first = page_to_sector_number (page_first);    // get starting sector of logged data
    sector_last = page_to_sector_number (page_last);      // get last sector of logged data

    for (int s = sector_first; s <= sector_last; s++)
    {
        copy_sector_to_local_buffer(s, buffer);         // copy sector content to local buffer

        b_read_pointer = g_read_pointer % (SECTOR_SIZE);  // convert global read pointer to local buffer read pointer

        // size of data to be copied depends if it ends in current sector
        if (s == sector_last)
        {
            local_size = size_remainder;  // data ends in current sector - copy only its tail
        }
        else
        {
            local_size = (s + 1) * SECTOR_SIZE - g_read_pointer;  // from read pointer to the end of sector
        }

        // fill data buffer from local buffer
        memcpy (data + (*size - size_remainder), buffer + b_read_pointer, local_size);

        size_remainder -= local_size;
        g_read_pointer += local_size;
        g_total_size -= local_size;
    }

    return 0;
}

int logger_write (unsigned char *data, unsigned int size)
{
    unsigned char buffer[SECTOR_SIZE];
    uint32_t page_first;
    uint32_t page_last;
    uint32_t page_start;
    uint16_t sector_first;
    uint16_t sector_last;
    uint16_t local_head;
    uint16_t local_size;
    uint16_t size_remainder = size;

    if (size == 0)
    {
        return -1;  // invalid data size
    }
    if ((g_total_size + size) > FLASH_SIZE)
    {
        return -2;  // flash memory is full
    }

    page_first = address_to_page_number (g_write_pointer);             // get starting page of logged data
    page_last  = address_to_page_number (g_write_pointer + size - 1);  // get last page of logged data

    sector_first = page_to_sector_number (page_first);     // get starting sector of logged data
    sector_last  = page_to_sector_number (page_last);      // get last sector of logged data

    for (int s = sector_first; s < sector_last; s++)
    {
        copy_sector_to_local_buffer (s, buffer);     // copy sector content to local buffer

        local_head = g_write_pointer % (SECTOR_SIZE);  // logged data in local buffer ends here

        // size of data to be copied depends if it ends in current sector
        if (s == sector_last)
        {
            local_size = size_remainder;  // data ends in current sector - copy only its tail
        }
        else
        {
            local_size = (s + 1) * SECTOR_SIZE - g_write_pointer;  // from data head to the end of sector
        }
        
        // append data to the local buffer
        memcpy (buffer + local_head, data + (size - size_remainder), local_size);

        flash_erase (s);  // erase the sector

        page_start = s * SECTOR_SIZE / PAGE_SIZE;  // convert sector number to its first page number

        // write updated sector pages back to flash
        for (int p = 0; p < SECTOR_SIZE / PAGE_SIZE; p++)
        {
            flash_write (page_start + p, buffer + p * PAGE_SIZE);
        }

        size_remainder -= local_size;
        g_write_pointer += local_size;
        g_total_size += local_size;
    }

    return 0;
}

/**
 * @brief Copies specific sector to given buffer and appends data
 *
 * This function copies specific sector to given buffer and appends data.
 *
 * @param sector  Sector number to be appended by data
 * @param buffer  Pointer to buffer where updated sector content will be copied to
 * @return 0 = ok, -1 = argument error, -2 = fail
 */
static void copy_sector_to_local_buffer (uint16_t sector, unsigned char *buffer)
{
    int page_start = sector * SECTOR_SIZE / PAGE_SIZE;  // convert sector number to its first page number

    // copy sector content to provided buffer
    for (int p = page_start; p < page_start + SECTOR_SIZE / PAGE_SIZE; p++)
    {
        flash_read (p, buffer + (p - page_start) * PAGE_SIZE);  // copy page to relevant place in buffer
    }
}

static uint32_t address_to_page_number (uint32_t address)
{
    if (address > FLASH_SIZE)
    {
        return -1;  // invalid address
    }
    return address / PAGE_SIZE;
}

static uint16_t page_to_sector_number (uint32_t page)
{
    if (page > LAST_PAGE)
    {
        return -1;  // invalid address
    }
    return page * PAGE_SIZE / (SECTOR_SIZE);
}

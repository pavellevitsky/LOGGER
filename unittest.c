#include <stdint.h>
#include <stdbool.h>
#include "configuration.h"
#include "logger.h"

static void initialise_buffer (uint8_t*, int, int);

bool test_flash_empty (void)
{
    bool rc;
    uint8_t buffer_read[512];
    unsigned int size = 32;

    logger_open (READ);
    initialise_buffer (buffer_read, sizeof(buffer_read), 0);
    rc = logger_read (buffer_read, &size) != 0;  // read must fail when flash is empty
    logger_close();

    return rc;
}

bool test_flash_full (void)
{
    uint8_t data[FLASH_SIZE/4 + 1];
    bool rc1, rc2, rc3, rc4;

    initialise_buffer (data, sizeof(data), 0);

    logger_open (WRITE);
    rc1 = logger_write (data, sizeof(data)) == 0;  // first write must pass
    rc2 = logger_write (data, sizeof(data)) == 0;  // second write must pass
    rc3 = logger_write (data, sizeof(data)) == 0;  // third write must pass
    rc4 = logger_write (data, sizeof(data)) != 0;  // fourth write must fail
    logger_close();

    return rc1 && rc2 && rc3 && rc4;
}

bool test_write_read (void)
{
    uint8_t buffer_128[128];
    uint8_t buffer_32[32];
    uint8_t buffer_77[77];
    uint8_t buffer_read[1024];
    unsigned int size;

    initialise_buffer(buffer_128, sizeof(buffer_128), 128);
    initialise_buffer(buffer_32, sizeof(buffer_32), 32);
    initialise_buffer(buffer_77, sizeof(buffer_77), 77);

    /* WRITE */
    logger_open(WRITE);

    logger_write(buffer_128, sizeof(buffer_128));
    logger_write(buffer_32, sizeof(buffer_32));
    logger_write(buffer_77, sizeof(buffer_77));

    logger_close();

    /* READ */
    logger_open(READ);

    size = 256;
    initialise_buffer(buffer_read, sizeof(buffer_read), 0);
    logger_read(buffer_read, &size);

    size = 1024;
    initialise_buffer(buffer_read, sizeof(buffer_read), 0);
    logger_read(buffer_read, &size);

    logger_close();

    return true;
}

bool test_read_all (void)
{
    return true;
}


static void initialise_buffer(uint8_t* buffer, int size, int value)
{
    for (int i = 0; i < size; i++)
    {
        buffer[i] = value;
    }
}

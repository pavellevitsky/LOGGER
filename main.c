#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "configuration.h"
#include "logger.h"

static void initialise_buffer (uint8_t *, int, int);

int main (int argc, const char * argv[])
{
    uint8_t buffer_128[128];
    uint8_t buffer_32[32];
    uint8_t buffer_77[77];
    uint8_t buffer_read[1024];
    unsigned int size;

    initialise_buffer (buffer_128, sizeof(buffer_128), 128);
    initialise_buffer (buffer_32, sizeof(buffer_32), 32);
    initialise_buffer (buffer_77, sizeof(buffer_77), 77);

    logger_init();

    /* WRITE */
    logger_open (WRITE);

    logger_write (buffer_128, sizeof(buffer_128));
    logger_write (buffer_32, sizeof(buffer_32));
    logger_write (buffer_77, sizeof(buffer_77));

    logger_close ();
    
    /* READ */
    logger_open (READ);

    size = 256;
    initialise_buffer (buffer_read, sizeof(buffer_read), 0);
    logger_read (buffer_read, &size);

    size = 1024;
    initialise_buffer (buffer_read, sizeof(buffer_read), 0);
    logger_read (buffer_read, &size);

    logger_close ();

    return 0;
}

static void initialise_buffer (uint8_t *buffer, int size, int value)
{
    for (int i = 0; i < size; i++)
    {
        buffer[i] = value;
    }
}

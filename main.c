#include <stdbool.h>
#include "logger.h"
#include "unittest.h"

int main (int argc, const char * argv[])
{
    bool rc_mode_validity;
    bool rc_flash_empty;
    bool rc_flash_full;
    bool rc_write_read;
    bool rc_read_all;
    bool rc;

    logger_init();

    // below tests sequence order is important !
    rc_mode_validity = test_mode_validity();
    rc_flash_empty   = test_flash_empty();
    rc_flash_full    = test_flash_full();
    logger_init();
    rc_write_read    = test_write_read();
    logger_init();
    rc_read_all      = test_read_all();

    rc = rc_mode_validity &&
         rc_flash_empty &&
         rc_flash_full &&
         rc_write_read &&
         rc_read_all;

    return rc ? 0 : 1;
}

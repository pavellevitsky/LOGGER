#include <stdbool.h>
#include "unittest.h"

int main (int argc, const char * argv[])
{
    int rc = 1;

    logger_init();

    if (test_flash_empty() &&
        test_flash_full() &&
        test_write_read() &&
        test_read_all())
    {
        rc = 0;
    }

    return rc;
}

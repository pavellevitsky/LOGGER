#include <stdbool.h>
#include <stdio.h>
#include "configuration.h"
#include "logger.h"
#include "unittest.h"

int main (int argc, const char * argv[])
{
    bool rc_mode_validity;
    bool rc_read_from_empty_flash;
    bool rc_write_to_full_flash;
    bool rc_write_read;
    bool rc_read_all;
    bool rc;

    logger_init();

    // below tests sequence order is important !
    rc_mode_validity         = test_mode_validity();
    printf("rc_mode_validity:%d\n", rc_mode_validity);
    rc_read_from_empty_flash = test_read_from_empty_flash();
    printf("rc_read_from_empty_flash:%d\n", rc_read_from_empty_flash);
    rc_write_to_full_flash   = test_write_to_full_flash();
    printf("rc_write_to_full_flash:%d\n", rc_write_to_full_flash);
    logger_init();
    rc_write_read = test_write_read();
    printf("rc_write_read:%d\n", rc_write_read);
    rc_write_read = test_write_read();
    printf("rc_write_read:%d\n", rc_write_read);
    rc_write_read = test_write_read();
    printf("rc_write_read:%d\n", rc_write_read);
//    rc_write_read = test_write_read();
    logger_init();
    rc_read_all   = test_read_all();
    printf("rc_read_all:%d\n", rc_read_all);

    rc = rc_mode_validity &&
         rc_read_from_empty_flash &&
         rc_write_to_full_flash &&
         rc_write_read &&
         rc_read_all;

    printf("[bool] rc:%d\n", rc);

    return rc ? 0 : 1;
}

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
    bool rc_write_read_1;
    bool rc_write_read_2;
    bool rc_write_read_3;
    bool rc_write_read_4;
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
    rc_write_read_1 = test_write_read();
    printf("rc_write_read_1:%d\n", rc_write_read_1);
    rc_write_read_2 = test_write_read();
    printf("rc_write_read_2:%d\n", rc_write_read_2);
    rc_write_read_3 = test_write_read();
    printf("rc_write_read_3:%d\n", rc_write_read_3);
    // rc_write_read_4 = test_write_read();
    // printf("rc_write_read_4:%d\n", rc_write_read_4);
    logger_init();
    rc_read_all   = test_read_all();
    printf("rc_read_all:%d\n", rc_read_all);

    rc = rc_mode_validity &&
         rc_read_from_empty_flash &&
         rc_write_to_full_flash &&
         rc_write_read_1 &&
         rc_write_read_2 &&
         rc_write_read_3 &&
         rc_read_all;

    printf("[bool] rc:%d\n", rc);

    return rc ? 0 : 1;
}

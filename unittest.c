#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "configuration.h"
#include "logger.h"

#define BUFFER_A_SIZE 64*16
#define BUFFER_B_SIZE 512*8
#define BUFFER_C_SIZE 77
#define BUFFER_READ_SIZE 1024*16

static bool test_mode_validity(void);
static bool test_read_from_empty_flash(void);
static bool test_write_to_full_flash(void);
static bool test_write_read(void);
static bool test_read_all(void);
static void initialise_buffer(uint8_t*, int, int);

int run_unit_tests(void)
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
    rc_mode_validity = test_mode_validity();
    rc_read_from_empty_flash = test_read_from_empty_flash();
    rc_write_to_full_flash = test_write_to_full_flash();

    logger_init();

    rc_write_read_1 = test_write_read();
    rc_write_read_2 = test_write_read();
    rc_write_read_3 = test_write_read();
    rc_write_read_4 = true;  // test_write_read();

    logger_init();

    rc_read_all = test_read_all();

    rc = rc_mode_validity &&
        rc_read_from_empty_flash &&
        rc_write_to_full_flash &&
        rc_write_read_1 &&
        rc_write_read_2 &&
        rc_write_read_3 &&
        rc_write_read_4 &&
        rc_read_all;

    printf("rc_mode_validity:%d\n", rc_mode_validity);
    printf("rc_read_from_empty_flash:%d\n", rc_read_from_empty_flash);
    printf("rc_write_to_full_flash:%d\n", rc_write_to_full_flash);
    printf("rc_write_read_1:%d\n", rc_write_read_1);
    printf("rc_write_read_2:%d\n", rc_write_read_2);
    printf("rc_write_read_3:%d\n", rc_write_read_3);
    printf("rc_write_read_4:%d (ignored)\n", rc_write_read_4);
    printf("rc_read_all:%d\n", rc_read_all);
    printf("[bool] rc:%d\n", rc);

    return rc ? 0 : 1;
}

static bool test_mode_validity (void)
{
    bool rc_unexpected_mode;
    bool rc_read_mode;
    bool rc_write_mode;
    bool rc_nesting_mode;

    rc_unexpected_mode = logger_open (IDLE) == -1;  // IDLE is not valid mode
    rc_read_mode       = logger_open (READ) == 0;   // set mode to READ
    logger_close();                                 // reset mode
    rc_write_mode      = logger_open (WRITE) == 0;  // set mode to WRITE
    rc_nesting_mode    = logger_open (READ) == -2;  // set mode to READ while active WRITE mode
    logger_close();                                 // reset mode

    return rc_unexpected_mode && rc_read_mode && rc_write_mode && rc_nesting_mode;
}

static bool test_read_from_empty_flash(void)
{
    bool rc;
    uint8_t buffer_read[BUFFER_READ_SIZE];
    unsigned int size = 32;

    initialise_buffer (buffer_read, sizeof(buffer_read), 0);  // initialise buffer; flash is empty now

    logger_open (READ);
    rc = logger_read (buffer_read, &size) != 0;  // reading from empty flash must fail
    logger_close();

    return rc;
}

static bool test_write_to_full_flash (void)
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

static bool test_write_read (void)
{
    uint8_t buffer_b[BUFFER_B_SIZE];
    uint8_t buffer_a[BUFFER_A_SIZE];
    uint8_t buffer_c[BUFFER_C_SIZE];
    uint8_t buffer_read[BUFFER_READ_SIZE];
    unsigned int size;
    bool rc1, rc2, rc3;

    initialise_buffer(buffer_b, sizeof(buffer_b), BUFFER_B_SIZE);
    initialise_buffer(buffer_a, sizeof(buffer_a), BUFFER_A_SIZE);
    initialise_buffer(buffer_c, sizeof(buffer_c), BUFFER_C_SIZE);

    /* WRITE */
    logger_open(WRITE);
    logger_write(buffer_b, sizeof(buffer_b));
    logger_write(buffer_a, sizeof(buffer_a));
    logger_write(buffer_c, sizeof(buffer_c));
    logger_close();

    /* READ */
    logger_open(READ);

    initialise_buffer(buffer_read, sizeof(buffer_read), 0);
    size = BUFFER_B_SIZE;
    logger_read(buffer_read, &size);
    rc1 = memcmp (buffer_read, buffer_b, BUFFER_B_SIZE) == 0;

    initialise_buffer(buffer_read, sizeof(buffer_read), 0);
    size = BUFFER_A_SIZE;
    logger_read(buffer_read, &size);
    rc2 = memcmp(buffer_read, buffer_a, BUFFER_A_SIZE) == 0;

    initialise_buffer(buffer_read, sizeof(buffer_read), 0);
    size = BUFFER_C_SIZE;
    logger_read(buffer_read, &size);
    rc3 = memcmp(buffer_read, buffer_c, BUFFER_C_SIZE) == 0;

    logger_close();

    return rc1 && rc2 && rc3;
}

static bool test_read_all (void)
{
    uint8_t buffer_a[BUFFER_A_SIZE];
    uint8_t buffer_b[BUFFER_B_SIZE];
    uint8_t buffer_read[BUFFER_READ_SIZE];
    unsigned int size = 2 * (BUFFER_A_SIZE + BUFFER_B_SIZE);  // try reading more data than was logged

    initialise_buffer(buffer_a, sizeof(buffer_a), BUFFER_A_SIZE);
    initialise_buffer(buffer_b, sizeof(buffer_b), BUFFER_B_SIZE);

    /* WRITE */
    logger_open(WRITE);
    logger_write(buffer_a, sizeof(buffer_a));
    logger_write(buffer_b, sizeof(buffer_b));
    logger_close();

    /* READ */
    logger_open(READ);
    initialise_buffer(buffer_read, sizeof(buffer_read), 0);
    logger_read(buffer_read, &size);
    logger_close();

    return (size == BUFFER_A_SIZE + BUFFER_B_SIZE);  // logged and read data sizes must be same
}

static void initialise_buffer(uint8_t* buffer, int size, int value)
{
    for (int i = 0; i < size; i++)
    {
        buffer[i] = value;
    }
}

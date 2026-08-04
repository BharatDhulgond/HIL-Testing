#include "test_i2c.h"
#include "test_runner.h"
#include "hal_i2c.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

#define MPU6050_ADDR        (0x68 << 1)
#define MPU6050_WHO_AM_I    0x75
#define MPU6050_PWR_MGMT_1  0x6B


typedef enum
{
    I2C_TEST_DEVICE_PRESENT,
    I2C_TEST_REGISTER_READ,
    I2C_TEST_WRONG_DEVICE
} i2c_test_case_t;


/* ---------- DEVICE IDENTIFICATION ---------- */

static const char* identify_device(uint8_t id)
{
    switch(id)
    {
        case 0x68: return "MPU6050";
        case 0x70: return "MPU6500";
        case 0x98: return "MPU CLONE";
        default:   return "UNKNOWN";
    }
}

static int is_valid_device(uint8_t id)
{
    return (id == 0x68 || id == 0x70 || id == 0x98);
}


/* ---------- Core I2C Operation ---------- */

static int i2c_read_id(uint16_t dev_addr, uint8_t *id)
{
    uint8_t wake = 0x00;

    /* Wake device */
    if(!hal_i2c_write(dev_addr, MPU6050_PWR_MGMT_1, &wake, 1))
        return 0;

    HAL_Delay(10);

    /* Read WHO_AM_I */
    if(!hal_i2c_read(dev_addr, MPU6050_WHO_AM_I, id, 1))
        return 0;

    return 1;
}


/* ---------- Test Execution ---------- */

static int run_i2c_test(i2c_test_case_t test)
{
    uint8_t id = 0;
    int status;

    switch(test)
    {
        case I2C_TEST_DEVICE_PRESENT:
        {
            status = i2c_read_id(MPU6050_ADDR, &id);

            if(!status)
            {
                printf("[I2C] FAIL: Device not responding\r\n");
                return 0;
            }

            if(is_valid_device(id))
            {
                printf("[I2C] PASS: %s detected (0x%02X)\r\n",
                       identify_device(id), id);
                return 1;
            }

            printf("[I2C] FAIL: Unknown device (0x%02X)\r\n", id);
            return 0;
        }

        case I2C_TEST_REGISTER_READ:
        {
            status = i2c_read_id(MPU6050_ADDR, &id);

            if(!status)
            {
                printf("[I2C] FAIL: Read operation failed\r\n");
                return 0;
            }

            if(id == 0x00 || id == 0xFF)
            {
                printf("[I2C] FAIL: Invalid register value (0x%02X)\r\n", id);
                return 0;
            }

            printf("[I2C] PASS: Register read OK (%s, 0x%02X)\r\n",
                   identify_device(id), id);

            return 1;
        }

        case I2C_TEST_WRONG_DEVICE:
        {
            status = hal_i2c_read(0x00,
                                 MPU6050_WHO_AM_I,
                                 &id,
                                 1);

            if(!status)
            {
                printf("[I2C] PASS: Invalid device correctly rejected\r\n");
                return 1;
            }

            printf("[I2C] FAIL: Unexpected response from invalid device\r\n");
            return 0;
        }

        default:
            printf("[I2C] FAIL: Unknown test case\r\n");
            return 0;
    }
}


/* ---------- Test Matrix ---------- */

typedef struct
{
    const char *name;
    i2c_test_case_t type;
} i2c_test_entry_t;


static const i2c_test_entry_t i2c_tests[] =
{
    {"I2C_DEVICE_PRESENT", I2C_TEST_DEVICE_PRESENT},
    {"I2C_REGISTER_READ",  I2C_TEST_REGISTER_READ},
    {"I2C_WRONG_DEVICE",   I2C_TEST_WRONG_DEVICE}
};


void run_i2c_matrix(void)
{
    printf("\r\n=== I2C TEST MATRIX ===\r\n\r\n");

    int count = sizeof(i2c_tests) / sizeof(i2c_tests[0]);

    for(int i = 0; i < count; i++)
    {
        printf("%s\r\n", i2c_tests[i].name);

        int pass = run_i2c_test(i2c_tests[i].type);

        printf("[TEST][I2C] RESULT: %s\r\n\r\n", pass ? "PASS" : "FAIL");

        report_result(pass);
    }
}

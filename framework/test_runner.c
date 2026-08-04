/*
 * test_runner.c
 *
 * HIL Test Framework Runner
 */

#include <stdio.h>
#include "test_runner.h"
#include "test_gpio.h"
#include "test_i2c.h"
#include "test_spi.h"

/* Test counters */
static int total_tests = 0;
static int total_failures = 0;


/* Called by individual tests */
void report_result(int passed)
{
    total_tests++;

    if (!passed)
        total_failures++;
}


/* Test suite definition */
typedef struct
{
    const char *name;
    void (*run)(void);
} test_suite_t;


/* Register all test suites here */
static const test_suite_t test_suites[] =
{
    {"GPIO TEST MATRIX", run_gpio_matrix},
    {"SPI TEST MATRIX",  run_spi_matrix},
    {"I2C TEST MATRIX",  run_i2c_matrix}
};

/* Main test execution */
void run_all_tests(void)
{
    total_tests = 0;
    total_failures = 0;

    printf("\r\n=================================\r\n");
    printf("     HIL TEST FRAMEWORK START\r\n");
    printf("=================================\r\n");

    int suite_count = sizeof(test_suites) / sizeof(test_suites[0]);

    for (int i = 0; i < suite_count; i++)
    {
        printf("\r\n=== %s ===\r\n", test_suites[i].name);

        if (test_suites[i].run != NULL)
        {
            test_suites[i].run();
        }
    }

    printf("\r\n=================================\r\n");
    printf("Total Tests : %d\r\n", total_tests);
    printf("Failures    : %d\r\n", total_failures);

    if (total_failures == 0)
        printf("OVERALL RESULT: PASS\r\n");
    else
        printf("OVERALL RESULT: FAIL\r\n");

    printf("=================================\r\n");
}

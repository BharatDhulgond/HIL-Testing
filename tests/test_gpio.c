#include "test_gpio.h"
#include "test_runner.h"
#include "hal_gpio.h"
#include "main.h"
#include <stdio.h>

#define GPIO_TEST_OUT_PORT ((uint32_t)GPIOC)
#define GPIO_TEST_OUT_PIN  GPIO_PIN_0

#define GPIO_TEST_IN_PORT  ((uint32_t)GPIOC)
#define GPIO_TEST_IN_PIN   GPIO_PIN_1


/* ---------- Stable Read ---------- */

static uint8_t stable_read(uint32_t port, uint16_t pin)
{
    int count = 0;

    for(int i = 0; i < 5; i++)
    {
        if(hal_gpio_read(port, pin))
            count++;

        HAL_Delay(1);
    }

    return (count >= 3);
}


/* ---------- Tests with Diagnostics ---------- */

static int gpio_test_high(void)
{
    hal_gpio_write(GPIO_TEST_OUT_PORT, GPIO_TEST_OUT_PIN, 1);
    HAL_Delay(5);

    uint8_t val = stable_read(GPIO_TEST_IN_PORT, GPIO_TEST_IN_PIN);

    if(val == 1)
    {
        printf("[GPIO] PASS: Output HIGH propagated correctly\r\n");
        return 1;
    }

    printf("[GPIO] FAIL: Expected HIGH, read LOW\r\n");
    return 0;
}


static int gpio_test_low(void)
{
    hal_gpio_write(GPIO_TEST_OUT_PORT, GPIO_TEST_OUT_PIN, 0);
    HAL_Delay(5);

    uint8_t val = stable_read(GPIO_TEST_IN_PORT, GPIO_TEST_IN_PIN);

    if(val == 0)
    {
        printf("[GPIO] PASS: Output LOW propagated correctly\r\n");
        return 1;
    }

    printf("[GPIO] FAIL: Expected LOW, read HIGH\r\n");
    return 0;
}


static int gpio_test_toggle(void)
{
    hal_gpio_write(GPIO_TEST_OUT_PORT, GPIO_TEST_OUT_PIN, 1);
    HAL_Delay(5);
    uint8_t a = stable_read(GPIO_TEST_IN_PORT, GPIO_TEST_IN_PIN);

    hal_gpio_write(GPIO_TEST_OUT_PORT, GPIO_TEST_OUT_PIN, 0);
    HAL_Delay(5);
    uint8_t b = stable_read(GPIO_TEST_IN_PORT, GPIO_TEST_IN_PIN);

    if(a == 1 && b == 0)
    {
        printf("[GPIO] PASS: Toggle behavior correct\r\n");
        return 1;
    }

    if(a == b)
    {
        printf("[GPIO] FAIL: No transition detected (stuck signal)\r\n");
    }
    else
    {
        printf("[GPIO] FAIL: Incorrect toggle response (a=%d, b=%d)\r\n", a, b);
    }

    return 0;
}


/* ---------- Test Matrix ---------- */

void run_gpio_matrix(void)
{
    printf("\r\n=== GPIO TEST MATRIX ===\r\n\r\n");

    int pass;

    printf("GPIO_HIGH\r\n");
    pass = gpio_test_high();
    printf("[TEST][GPIO] RESULT: %s\r\n\r\n", pass ? "PASS" : "FAIL");
    report_result(pass);

    printf("GPIO_LOW\r\n");
    pass = gpio_test_low();
    printf("[TEST][GPIO] RESULT: %s\r\n\r\n", pass ? "PASS" : "FAIL");
    report_result(pass);

    printf("GPIO_TOGGLE\r\n");
    pass = gpio_test_toggle();
    printf("[TEST][GPIO] RESULT: %s\r\n\r\n", pass ? "PASS" : "FAIL");
    report_result(pass);
}

#include "hal_gpio.h"
#include "main.h"
#include "ai_generated_embedded.h"

void hal_gpio_write(uint32_t port, uint16_t pin, uint8_t value)
{
    ai_gpio_write(port, pin, value);
}

uint8_t hal_gpio_read(uint32_t port, uint16_t pin)
{
    return ai_gpio_read(port, pin);
}

#ifndef HAL_GPIO_IFACE_H
#define HAL_GPIO_IFACE_H

#include <stdint.h>

void hal_gpio_write(uint32_t port, uint16_t pin, uint8_t value);
uint8_t hal_gpio_read(uint32_t port, uint16_t pin);

#endif

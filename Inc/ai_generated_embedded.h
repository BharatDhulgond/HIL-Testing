#ifndef AI_GENERATED_EMBEDDED_H
#define AI_GENERATED_EMBEDDED_H

#include <stdint.h>

/* GPIO */
int ai_gpio_write(uint32_t port, uint16_t pin, uint8_t val);
uint8_t ai_gpio_read(uint32_t port, uint16_t pin);

/* SPI */
int ai_spi_transfer(uint8_t *tx, uint8_t *rx, uint16_t len);

/* I2C */
int ai_i2c_read(uint8_t dev, uint8_t reg, uint8_t *data, uint16_t len);
int ai_i2c_write(uint8_t dev, uint8_t reg, uint8_t *data, uint16_t len);

#endif

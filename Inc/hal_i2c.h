#ifndef HAL_I2C_IFACE_H
#define HAL_I2C_IFACE_H

#include <stdint.h>

int hal_i2c_read(uint8_t dev,
                 uint8_t reg,
                 uint8_t *data,
                 uint16_t len);

#endif

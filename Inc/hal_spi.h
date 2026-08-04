#ifndef HAL_SPI_IFACE_H
#define HAL_SPI_IFACE_H

#include <stdint.h>

int hal_spi_transfer(uint8_t *tx, uint8_t *rx, uint16_t len);

#endif

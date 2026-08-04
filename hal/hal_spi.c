/*
 * hal_spi.c
 *
 * Hardware Abstraction Layer for SPI
 */

#include "hal_spi.h"
#include "fault_injection.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_spi.h"
#include "ai_generated_embedded.h"

extern SPI_HandleTypeDef hspi1;


/* Generic SPI transfer wrapper */
int hal_spi_transfer(uint8_t *tx, uint8_t *rx, uint16_t len)
{
    /* Basic parameter validation */
    if (tx == NULL || rx == NULL || len == 0)
        return 0;

    /* Inject TX corruption before transfer */
    fault_inject_tx(tx, len);

    /* Simulate timeout fault */
    if (fault_get() == FI_TIMEOUT)
    {
        HAL_Delay(100);
        return 0;
    }

//    HAL_StatusTypeDef status =
//        HAL_SPI_TransmitReceive(&hspi1, tx, rx, len, 100);
    int status = ai_spi_transfer(tx, rx, len);

    if (!status)
        return 0;

    /* Inject RX corruption after transfer */
    fault_inject_rx(rx, len);

    return 1;
}

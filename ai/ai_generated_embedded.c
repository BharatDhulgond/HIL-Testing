#include "ai_generated_embedded.h"
#include "stm32f4xx_hal.h"

extern SPI_HandleTypeDef hspi1;
extern I2C_HandleTypeDef hi2c1;

/* ===================================================== */
/* ================= GPIO SECTION ======================= */
/* ===================================================== */

int ai_gpio_write(uint32_t port, uint16_t pin, uint8_t val)
{
    /* AI-generated logic */
    if(val)
    {
        HAL_GPIO_WritePin((GPIO_TypeDef*)port, pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin((GPIO_TypeDef*)port, pin, GPIO_PIN_RESET);
    }

    return 1;
}

uint8_t ai_gpio_read(uint32_t port, uint16_t pin)
{
    GPIO_PinState state;

    state = HAL_GPIO_ReadPin((GPIO_TypeDef*)port, pin);

    if(state == GPIO_PIN_SET)
        return 1;
    else
        return 0;
}


/* ===================================================== */
/* ================= SPI SECTION ======================== */
/* ===================================================== */

int ai_spi_transfer(uint8_t *tx, uint8_t *rx, uint16_t len)
{
    HAL_StatusTypeDef status;

    if(tx == NULL || rx == NULL || len == 0)
        return 0;

    /* AI-style direct HAL usage */
    status = HAL_SPI_TransmitReceive(&hspi1, tx, rx, len, 100);

    if(status != HAL_OK)
        return 0;

    return 1;
}


/* ===================================================== */
/* ================= I2C SECTION ======================== */
/* ===================================================== */

int ai_i2c_write(uint8_t dev, uint8_t reg, uint8_t *data, uint16_t len)
{
    HAL_StatusTypeDef status;

    if(data == NULL || len == 0)
        return 0;

    status = HAL_I2C_Mem_Write(&hi2c1,
                              dev,
                              reg,
                              I2C_MEMADD_SIZE_8BIT,
                              data,
                              len,
                              100);

    if(status != HAL_OK)
        return 0;

    return 1;
}


int ai_i2c_read(uint8_t dev, uint8_t reg, uint8_t *data, uint16_t len)
{
    HAL_StatusTypeDef status;

    if(data == NULL || len == 0)
        return 0;

    status = HAL_I2C_Mem_Read(&hi2c1,
                             dev,
                             reg,
                             I2C_MEMADD_SIZE_8BIT,
                             data,
                             len,
                             100);

    if(status != HAL_OK)
        return 0;

    return 1;
}

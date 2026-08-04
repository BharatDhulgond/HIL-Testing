#include "hal_i2c.h"
#include "main.h"
#include "fault_injection.h"
#include <stdio.h>
#include "ai_generated_embedded.h"

extern I2C_HandleTypeDef hi2c1;


///* ---------- I2C READ ---------- */
//
//int hal_i2c_read(uint8_t dev,
//                 uint8_t reg,
//                 uint8_t *data,
//                 uint16_t len)
//{
//    /* Future fault injection hook */
//    if(fault_get() == FI_TIMEOUT)
//    {
//        HAL_Delay(200);
//        return 0;
//    }
//
//    HAL_StatusTypeDef status =
//        HAL_I2C_Mem_Read(&hi2c1,
//                         dev,
//                         reg,
//                         I2C_MEMADD_SIZE_8BIT,
//                         data,
//                         len,
//                         100);
//
//    return (status == HAL_OK);
//}
int hal_i2c_read(uint8_t dev,
                 uint8_t reg,
                 uint8_t *data,
                 uint16_t len)
{
    if(fault_get() == FI_TIMEOUT)
    {
        HAL_Delay(200);
        return 0;
    }

    return ai_i2c_read(dev, reg, data, len);
}

/* ---------- I2C WRITE ---------- */

//int hal_i2c_write(uint8_t dev,
//                  uint8_t reg,
//                  uint8_t *data,
//                  uint16_t len)
//{
//    HAL_StatusTypeDef status =
//        HAL_I2C_Mem_Write(&hi2c1,
//                          dev,
//                          reg,
//                          I2C_MEMADD_SIZE_8BIT,
//                          data,
//                          len,
//                          100);
//
//    return (status == HAL_OK);
//}
int hal_i2c_write(uint8_t dev,
                  uint8_t reg,
                  uint8_t *data,
                  uint16_t len)
{
    return ai_i2c_write(dev, reg, data, len);
}

#include "tmp102.h"
#include <stdio.h>
#include <string.h>
extern UART_HandleTypeDef huart2;

extern I2C_HandleTypeDef hi2c1;


HAL_StatusTypeDef TMP102_Init(void)
	{
	    if (HAL_I2C_IsDeviceReady(&hi2c1, TMP102_ADDR, 2, 100) != HAL_OK)
	    {
	        return HAL_ERROR;
	    }
	    uint8_t pData[3] = { TMP102_REG_CONFIG, 0x60, 0xA0 };

	    if (HAL_I2C_Master_Transmit(&hi2c1, TMP102_ADDR, pData, 3, 100) != HAL_OK)
	    {
	        return HAL_ERROR;
	    }

	    uint8_t reg = TMP102_REG_CONFIG;
	        uint8_t lu[2];

	        HAL_I2C_Master_Transmit(&hi2c1, TMP102_ADDR, &reg, 1, 100);
	        HAL_I2C_Master_Receive(&hi2c1, TMP102_ADDR, lu, 2, 100);


	        char msg[60];
	        sprintf(msg, "Config TMP102 : 0x%02X 0x%02X\r\n", lu[0], lu[1]);
	        HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);

	        return HAL_OK;
	}

HAL_StatusTypeDef TMP102_Measure(int16_t *raw_temp)
{
    uint8_t data[2] = {0};
    uint8_t reg = TMP102_REG_TEMP;


    if (HAL_I2C_Master_Transmit(&hi2c1, TMP102_ADDR, &reg, 1, 100) != HAL_OK)
    {
        return HAL_ERROR;
    }


    if (HAL_I2C_Master_Receive(&hi2c1, TMP102_ADDR, data, 2, 100) != HAL_OK)
    {
        return HAL_ERROR;
    }


    *raw_temp = (int16_t)((data[0] << 4) | (data[1] >> 4));


    if (*raw_temp & 0x800)
    {
        *raw_temp |= 0xF000;
    }

    return HAL_OK;
}


float TMP102_RawToCelsius(int16_t raw_temp)
{

    return (float)raw_temp * 0.0625f;
}


int32_t TMP102_RawToMilliC(int16_t raw_temp)
{

    return ((int32_t)raw_temp * 1000) / 16;
}

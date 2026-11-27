#ifndef TMP102_H
#define TMP102_H

#include "main.h"


#define TMP102_ADDR (0x48 << 1)


#define TMP102_REG_TEMP 0x00
#define TMP102_REG_CONFIG 0x01


HAL_StatusTypeDef TMP102_Init(void);
HAL_StatusTypeDef TMP102_Measure(int16_t *raw_temp);
float TMP102_RawToCelsius(int16_t raw_temp);
int32_t TMP102_RawToMilliC(int16_t raw_temp);

#endif

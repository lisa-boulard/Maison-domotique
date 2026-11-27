/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BT_MODE_Pin GPIO_PIN_13
#define BT_MODE_GPIO_Port GPIOC
#define BT_MODE_EXTI_IRQn EXTI15_10_IRQn
#define PIR_Pin GPIO_PIN_0
#define PIR_GPIO_Port GPIOA
#define PIR_EXTI_IRQn EXTI0_IRQn
#define LED_INTRUSION_Pin GPIO_PIN_1
#define LED_INTRUSION_GPIO_Port GPIOA
#define BUZZER_Pin GPIO_PIN_4
#define BUZZER_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_5
#define LED_GPIO_Port GPIOA
#define SENSOR_Pin GPIO_PIN_7
#define SENSOR_GPIO_Port GPIOA
#define GAZ_DETECTOR_Pin GPIO_PIN_0
#define GAZ_DETECTOR_GPIO_Port GPIOB
#define BT_FENETRE_Pin GPIO_PIN_11
#define BT_FENETRE_GPIO_Port GPIOC
#define BT_FENETRE_EXTI_IRQn EXTI15_10_IRQn
#define BT_MODE2_Pin GPIO_PIN_12
#define BT_MODE2_GPIO_Port GPIOC
#define BT_MODE2_EXTI_IRQn EXTI15_10_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

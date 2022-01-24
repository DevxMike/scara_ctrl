/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DB4_Pin GPIO_PIN_0
#define DB4_GPIO_Port GPIOC
#define DB5_Pin GPIO_PIN_1
#define DB5_GPIO_Port GPIOC
#define DB6_Pin GPIO_PIN_2
#define DB6_GPIO_Port GPIOC
#define DB7_Pin GPIO_PIN_3
#define DB7_GPIO_Port GPIOC
#define DIR_Pin GPIO_PIN_5
#define DIR_GPIO_Port GPIOA
#define SLEEP_Pin GPIO_PIN_7
#define SLEEP_GPIO_Port GPIOA
#define RW_Pin GPIO_PIN_7
#define RW_GPIO_Port GPIOC
#define RS_Pin GPIO_PIN_8
#define RS_GPIO_Port GPIOC
#define E_Pin GPIO_PIN_9
#define E_GPIO_Port GPIOC
#define REL_Pin GPIO_PIN_10
#define REL_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

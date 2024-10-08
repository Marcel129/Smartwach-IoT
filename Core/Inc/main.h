/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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
#define BUILDIN_LED_Pin GPIO_PIN_13
#define BUILDIN_LED_GPIO_Port GPIOC
#define SENSOR_2_Pin GPIO_PIN_0
#define SENSOR_2_GPIO_Port GPIOB
#define SENSOR_1_Pin GPIO_PIN_1
#define SENSOR_1_GPIO_Port GPIOB
#define RIGHT_BUTTON_Pin GPIO_PIN_13
#define RIGHT_BUTTON_GPIO_Port GPIOB
#define LEFT_BUTTON_Pin GPIO_PIN_14
#define LEFT_BUTTON_GPIO_Port GPIOB
#define RESET_Pin GPIO_PIN_15
#define RESET_GPIO_Port GPIOB
#define NSS_Pin GPIO_PIN_8
#define NSS_GPIO_Port GPIOA
#define DIO0_Pin GPIO_PIN_9
#define DIO0_GPIO_Port GPIOA
#define MODE_Pin GPIO_PIN_10
#define MODE_GPIO_Port GPIOA
#define BLUE_LED_Pin GPIO_PIN_11
#define BLUE_LED_GPIO_Port GPIOA
#define GREEN_LED_Pin GPIO_PIN_12
#define GREEN_LED_GPIO_Port GPIOA
#define RED_LED_Pin GPIO_PIN_5
#define RED_LED_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

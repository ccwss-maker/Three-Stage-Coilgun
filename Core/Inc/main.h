/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
extern uint8_t test_sign;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define K_ZVS_Pin GPIO_PIN_13
#define K_ZVS_GPIO_Port GPIOC
#define K_L0_Pin GPIO_PIN_0
#define K_L0_GPIO_Port GPIOC
#define K_L1_Pin GPIO_PIN_1
#define K_L1_GPIO_Port GPIOC
#define WLAN_EN_Pin GPIO_PIN_2
#define WLAN_EN_GPIO_Port GPIOC
#define V_2_Pin GPIO_PIN_3
#define V_2_GPIO_Port GPIOC
#define TFT_LED_Pin GPIO_PIN_0
#define TFT_LED_GPIO_Port GPIOA
#define G1_2_Pin GPIO_PIN_1
#define G1_2_GPIO_Port GPIOA
#define G0_2_Pin GPIO_PIN_2
#define G0_2_GPIO_Port GPIOA
#define G2_2_Pin GPIO_PIN_3
#define G2_2_GPIO_Port GPIOA
#define TFT_CS_Pin GPIO_PIN_4
#define TFT_CS_GPIO_Port GPIOA
#define G1_1_Pin GPIO_PIN_5
#define G1_1_GPIO_Port GPIOA
#define G0_1_Pin GPIO_PIN_6
#define G0_1_GPIO_Port GPIOA
#define V_0_Pin GPIO_PIN_7
#define V_0_GPIO_Port GPIOA
#define G1_0_Pin GPIO_PIN_4
#define G1_0_GPIO_Port GPIOC
#define G0_0_Pin GPIO_PIN_5
#define G0_0_GPIO_Port GPIOC
#define V_1_Pin GPIO_PIN_0
#define V_1_GPIO_Port GPIOB
#define KF_0_Pin GPIO_PIN_1
#define KF_0_GPIO_Port GPIOB
#define KF_1_Pin GPIO_PIN_2
#define KF_1_GPIO_Port GPIOB
#define KC_1_Pin GPIO_PIN_10
#define KC_1_GPIO_Port GPIOB
#define T_CS_Pin GPIO_PIN_12
#define T_CS_GPIO_Port GPIOB
#define T_SCK_Pin GPIO_PIN_13
#define T_SCK_GPIO_Port GPIOB
#define T_MISO_Pin GPIO_PIN_14
#define T_MISO_GPIO_Port GPIOB
#define T_MOSI_Pin GPIO_PIN_15
#define T_MOSI_GPIO_Port GPIOB
#define KC_2_Pin GPIO_PIN_6
#define KC_2_GPIO_Port GPIOC
#define KF_2_Pin GPIO_PIN_7
#define KF_2_GPIO_Port GPIOC
#define T_IQR_Pin GPIO_PIN_8
#define T_IQR_GPIO_Port GPIOC
#define SD_T_Pin GPIO_PIN_9
#define SD_T_GPIO_Port GPIOC
#define K_R1_Pin GPIO_PIN_8
#define K_R1_GPIO_Port GPIOA
#define KC_0_Pin GPIO_PIN_11
#define KC_0_GPIO_Port GPIOA
#define K_R0_Pin GPIO_PIN_12
#define K_R0_GPIO_Port GPIOA
#define SD_CS_Pin GPIO_PIN_15
#define SD_CS_GPIO_Port GPIOA
#define SD_SCK_Pin GPIO_PIN_10
#define SD_SCK_GPIO_Port GPIOC
#define SD_MISO_Pin GPIO_PIN_11
#define SD_MISO_GPIO_Port GPIOC
#define SD_MOSI_Pin GPIO_PIN_12
#define SD_MOSI_GPIO_Port GPIOC
#define DJ0_Pin GPIO_PIN_2
#define DJ0_GPIO_Port GPIOD
#define TFT_SCK_Pin GPIO_PIN_3
#define TFT_SCK_GPIO_Port GPIOB
#define DJ1_Pin GPIO_PIN_4
#define DJ1_GPIO_Port GPIOB
#define TFT_MOSI_Pin GPIO_PIN_5
#define TFT_MOSI_GPIO_Port GPIOB
#define DJ2_Pin GPIO_PIN_6
#define DJ2_GPIO_Port GPIOB
#define TFT_RST_Pin GPIO_PIN_7
#define TFT_RST_GPIO_Port GPIOB
#define TFT_DC_Pin GPIO_PIN_8
#define TFT_DC_GPIO_Port GPIOB
#define DJ_3_Pin GPIO_PIN_9
#define DJ_3_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

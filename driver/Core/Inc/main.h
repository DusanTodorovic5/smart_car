/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
extern uint8_t right_direction_lights;
extern uint8_t left_direction_lights;
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
#define left_dir_light_Pin GPIO_PIN_5
#define left_dir_light_GPIO_Port GPIOE
#define back_sensor_input_Pin GPIO_PIN_0
#define back_sensor_input_GPIO_Port GPIOC
#define front_sensor_output_Pin GPIO_PIN_1
#define front_sensor_output_GPIO_Port GPIOC
#define front_sensor_input_Pin GPIO_PIN_2
#define front_sensor_input_GPIO_Port GPIOC
#define input4_Pin GPIO_PIN_3
#define input4_GPIO_Port GPIOC
#define light_sensor_Pin GPIO_PIN_3
#define light_sensor_GPIO_Port GPIOA
#define input1_Pin GPIO_PIN_4
#define input1_GPIO_Port GPIOA
#define lights_Pin GPIO_PIN_5
#define lights_GPIO_Port GPIOA
#define stop_lights_Pin GPIO_PIN_7
#define stop_lights_GPIO_Port GPIOA
#define input2_Pin GPIO_PIN_4
#define input2_GPIO_Port GPIOC
#define right_dir_light_Pin GPIO_PIN_0
#define right_dir_light_GPIO_Port GPIOB
#define input3_Pin GPIO_PIN_1
#define input3_GPIO_Port GPIOB
#define motor_pwm1_Pin GPIO_PIN_11
#define motor_pwm1_GPIO_Port GPIOE
#define motor_pwm2_Pin GPIO_PIN_13
#define motor_pwm2_GPIO_Port GPIOE
#define back_sensor_output_Pin GPIO_PIN_3
#define back_sensor_output_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */
// extern uint8_t right_direction_lights;
// extern uint8_t left_direction_lights;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

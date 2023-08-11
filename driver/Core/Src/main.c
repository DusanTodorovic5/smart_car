/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdio.h"
#include "string.h"
#include "messages.h"

#define LIGHT_SENSOR_TRESHOLD 120
/*2.8*0.0343/2*/
#define SPEED_OF_SOUND 0.04802

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim5;

UART_HandleTypeDef huart4;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM5_Init(void);
static void MX_ADC1_Init(void);
static void MX_UART4_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char uartBuf[100];

char rcvBuf[100];

/* USER CODE END 0 */
uint8_t light_sensor_check() {
  HAL_ADC_Start(&hadc1);
  HAL_ADC_PollForConversion(&hadc1, 1);

  uint32_t val = HAL_ADC_GetValue(&hadc1);

  // sprintf(uartBuf, "Analog value  = %d \r\n", (int)val);
  // HAL_UART_Transmit(&huart4, (uint8_t *)uartBuf, strlen(uartBuf), 100);

  return val < LIGHT_SENSOR_TRESHOLD;
}

void usDelay(uint32_t sec) {
  if (sec < 2) {
    sec = 2;
  }

  TIM5->ARR = sec - 1;
  TIM5->EGR = 1;
  TIM5->SR &= ~1;
  TIM5->CR1 |= 1;
  while ((TIM5->SR & 0x0001) != 1);
  TIM5->SR &= ~(0x0001);
}

float front_distance_check_sensor() {
    static int numTicks;
    static float distance;
    numTicks = 0;

    HAL_GPIO_WritePin(front_sensor_output_GPIO_Port, front_sensor_output_Pin, GPIO_PIN_RESET);
    usDelay(3);

    HAL_GPIO_WritePin(front_sensor_output_GPIO_Port, front_sensor_output_Pin, GPIO_PIN_SET);
    usDelay(10);
    HAL_GPIO_WritePin(front_sensor_output_GPIO_Port, front_sensor_output_Pin, GPIO_PIN_RESET);

    while (HAL_GPIO_ReadPin(front_input_sensor_GPIO_Port, front_input_sensor_Pin) == GPIO_PIN_RESET);

    while (HAL_GPIO_ReadPin(front_input_sensor_GPIO_Port, front_input_sensor_Pin) == GPIO_PIN_SET) {
      ++numTicks;
      usDelay(2);
    }

    // sleep_duration = sleep_scaled(numTicks);

    distance = (numTicks + 0.0f) * SPEED_OF_SOUND;
		
		//5. Print to UART terminal for debugging
		// sprintf(uartBuf, "Front Distance (cm)  = %d \r\nFront NUM TICKS: %d \r\n", (int)(distance*10),(int)numTicks);
		// HAL_UART_Transmit(&huart4, (uint8_t *)uartBuf, strlen(uartBuf), 100);

    return distance;
}

float rear_distance_check_sensor() {
    static int numTicks;
    static float distance;
    numTicks = 0;

    
  HAL_GPIO_WritePin(back_sensor_output_GPIO_Port, back_sensor_output_Pin, GPIO_PIN_RESET);
    usDelay(3);

    HAL_GPIO_WritePin(back_sensor_output_GPIO_Port, back_sensor_output_Pin, GPIO_PIN_SET);
    usDelay(10);
    HAL_GPIO_WritePin(back_sensor_output_GPIO_Port, back_sensor_output_Pin, GPIO_PIN_RESET);

    while (HAL_GPIO_ReadPin(back_sensor_input_GPIO_Port, back_sensor_input_Pin) == GPIO_PIN_RESET);

    while (HAL_GPIO_ReadPin(back_sensor_input_GPIO_Port, back_sensor_input_Pin) == GPIO_PIN_SET) {
      ++numTicks;
      usDelay(2);
    }

    // sleep_duration = sleep_scaled(numTicks);

    distance = (numTicks + 0.0f) * SPEED_OF_SOUND;
		
		//5. Print to UART terminal for debugging
		// sprintf(uartBuf, "Rear Distance (cm)  = %d \r\nFront NUM TICKS: %d \r\n", (int)(distance*10),(int)numTicks);
		// HAL_UART_Transmit(&huart4, (uint8_t *)uartBuf, strlen(uartBuf), 100);

    return distance;
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  memset(rcvBuf, 0, 100 * sizeof(char));
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM5_Init();
  MX_ADC1_Init();
  MX_UART4_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  // Engine
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
  // Direction
  // HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  // __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 625);

  uint8_t auto_lights = 0;

  int x;
  while (1)
  {
    HAL_GPIO_TogglePin(motor_relay_GPIO_Port, motor_relay_Pin);
    for (x=0; x< 625; x++) {
      __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, x);
      HAL_Delay(6);
    }

    for (x = 625; x > 0; x--) {
       __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, x);
      HAL_Delay(6);
    }
    // HAL_UART_Receive(&huart4, rcvBuf, 1, 50);

    // switch (((uint8_t)(rcvBuf[0])))
    // {
    // case 1:
    // {
    //   /* voltage_engine_message */
    //   HAL_UART_Receive(&huart4, &(rcvBuf[1]), sizeof(voltage_engine_message) - 1, 50);
    //   voltage_engine_message* msg = (voltage_engine_message *) rcvBuf;

    //   sprintf(uartBuf, "TYPE 1 = %d\r\n",
    //             msg->voltage);
		//   HAL_UART_Transmit(&huart4, (uint8_t *)uartBuf, strlen(uartBuf), 100);

    //   if (msg->voltage > 100) {
    //     HAL_GPIO_WritePin(motor_relay_GPIO_Port, motor_relay_Pin, GPIO_PIN_SET);
    //     msg->voltage = 255 - msg->voltage;
    //   } else {
    //     HAL_GPIO_WritePin(motor_relay_GPIO_Port, motor_relay_Pin, GPIO_PIN_RESET);
    //   }

    //   int voltage = (625 / 100) * msg->voltage;
    //   __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, voltage);

    // }
    //   break;
    // case 2:
    // {
    //   /* direction_change_message */
    //   HAL_UART_Receive(&huart4, &(rcvBuf[1]), sizeof(direction_change_message) - 1, 50);
    //   direction_change_message* msg = (direction_change_message *) rcvBuf;

    //   sprintf(uartBuf, "TYPE 2 = %d\r\n",
    //             msg->direction);
		// HAL_UART_Transmit(&huart4, (uint8_t *)uartBuf, strlen(uartBuf), 100);


    //   if (msg->direction > 100) {
    //     HAL_GPIO_WritePin(direction_relay_GPIO_Port, direction_relay_Pin, GPIO_PIN_SET);
    //     msg->direction = 255 - msg->direction;
    //   } else {
    //     HAL_GPIO_WritePin(direction_relay_GPIO_Port, direction_relay_Pin, GPIO_PIN_RESET);
    //   }

    //   int voltage = (625 / 100) * msg->direction;
    //   __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, voltage);
    // }
    //   break;
    // case 3:
    // {
    //   /* led_change_message */
    //   HAL_UART_Receive(&huart4, &(rcvBuf[1]), sizeof(led_change_message) - 1, 50);
    //   led_change_message* msg = (led_change_message *) rcvBuf;

    //   sprintf(uartBuf, "TYPE 3 = %d, %d, %d, %d \r\n",
    //             msg->auto_lights,
    //             msg->front_light,
    //             msg->left_dir_light,
    //             msg->right_dir_light);
		// HAL_UART_Transmit(&huart4, (uint8_t *)uartBuf, strlen(uartBuf), 100);

    //   HAL_GPIO_WritePin(lights_GPIO_Port, lights_Pin, msg->front_light ? GPIO_PIN_SET : GPIO_PIN_RESET);
    //   HAL_GPIO_WritePin(left_dir_light_GPIO_Port, left_dir_light_Pin, msg->left_dir_light ? GPIO_PIN_SET : GPIO_PIN_RESET);
    //   HAL_GPIO_WritePin(right_dir_light_GPIO_Port, right_dir_light_Pin, msg->right_dir_light ? GPIO_PIN_SET : GPIO_PIN_RESET);
    //   auto_lights = msg->auto_lights ? 1 : 0;
    // }
    //   break;
    // }

    // if (auto_lights && light_sensor_check()) {
    //   HAL_GPIO_WritePin(lights_GPIO_Port, lights_Pin, GPIO_PIN_SET);
    // } else {
    //   HAL_GPIO_WritePin(lights_GPIO_Port, lights_Pin, GPIO_PIN_RESET);
    // }

    // front_distance_check_sensor();
    // rear_distance_check_sensor();
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 127;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 625;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 127;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 625;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 0;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 4294967295;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(left_dir_light_GPIO_Port, left_dir_light_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, motor_relay_Pin|direction_relay_Pin|front_sensor_output_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, back_sensor_output_Pin|lights_Pin|stop_lights_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(right_dir_light_GPIO_Port, right_dir_light_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : left_dir_light_Pin */
  GPIO_InitStruct.Pin = left_dir_light_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(left_dir_light_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : motor_relay_Pin direction_relay_Pin front_sensor_output_Pin */
  GPIO_InitStruct.Pin = motor_relay_Pin|direction_relay_Pin|front_sensor_output_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : front_input_sensor_Pin */
  GPIO_InitStruct.Pin = front_input_sensor_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(front_input_sensor_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : back_sensor_output_Pin lights_Pin stop_lights_Pin */
  GPIO_InitStruct.Pin = back_sensor_output_Pin|lights_Pin|stop_lights_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : right_dir_light_Pin */
  GPIO_InitStruct.Pin = right_dir_light_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(right_dir_light_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : back_sensor_input_Pin */
  GPIO_InitStruct.Pin = back_sensor_input_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(back_sensor_input_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

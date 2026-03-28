/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LED_NUM 5
#define PWM_DATA_SIZE 24*LED_NUM+50
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
DMA_HandleTypeDef hdma_tim4_ch1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t rx_byte;
uint8_t rx_buff[4] = {0x00, 0x00, 0x00, 0xFF};
uint16_t rx_idx = 0;

const uint16_t ROW_PIN[8] = {
  ROW8_Pin,
  ROW7_Pin,
  ROW6_Pin,
  ROW5_Pin,
  ROW4_Pin,
  ROW3_Pin,
  ROW2_Pin,
  ROW1_Pin,
};

const GPIO_TypeDef* ROW_PORT[8] = {
  ROW8_GPIO_Port,
  ROW7_GPIO_Port,
  ROW6_GPIO_Port,
  ROW5_GPIO_Port,
  ROW4_GPIO_Port,
  ROW3_GPIO_Port,
  ROW2_GPIO_Port,
  ROW1_GPIO_Port,
};

const uint16_t COL_PIN[3] = {
  COL1_Pin,
  COL2_Pin,
  COL3_Pin,
};

const GPIO_TypeDef* COL_PORT[3] = {
  COL1_GPIO_Port,
  COL2_GPIO_Port,
  COL3_GPIO_Port,
};

volatile uint8_t current_row = 0;

uint8_t frame_buff[3] = {
		0x00,
		0x00,
		0x00
};

uint16_t pwm_data[PWM_DATA_SIZE];

uint8_t LED_Data[LED_NUM][3];

uint8_t led_data_sent = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */
static inline uint8_t map(uint8_t value, uint8_t in_min, uint8_t in_max, uint8_t out_min, uint8_t out_max);
static inline uint8_t map_byte(uint8_t value, uint8_t in_min, uint8_t in_max, uint8_t out_min, uint8_t out_max);
void gauge(const uint8_t val, const uint8_t col, const uint8_t level_min,  const uint8_t level_max);
void matrix_display(const uint8_t setup[8][3]);
void send_ws();
void set_led(const uint8_t led_index, const uint8_t r, const uint8_t g, const uint8_t b);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart2, &rx_byte, 1);
  HAL_TIM_Base_Start_IT(&htim3);

  HAL_GPIO_WritePin(COL1_GPIO_Port, COL1_Pin, 1);
  HAL_GPIO_WritePin(COL2_GPIO_Port, COL2_Pin, 1);
  HAL_GPIO_WritePin(COL3_GPIO_Port, COL3_Pin, 1);

  HAL_GPIO_WritePin(ROW1_GPIO_Port, ROW1_Pin, 1);
  HAL_GPIO_WritePin(ROW2_GPIO_Port, ROW2_Pin, 1);
  HAL_GPIO_WritePin(ROW3_GPIO_Port, ROW3_Pin, 1);
  HAL_GPIO_WritePin(ROW4_GPIO_Port, ROW4_Pin, 1);
  HAL_GPIO_WritePin(ROW5_GPIO_Port, ROW5_Pin, 1);
  HAL_GPIO_WritePin(ROW6_GPIO_Port, ROW6_Pin, 1);
  HAL_GPIO_WritePin(ROW7_GPIO_Port, ROW7_Pin, 1);
  HAL_GPIO_WritePin(ROW8_GPIO_Port, ROW8_Pin, 1);

  set_led(0, 0xFF, 0, 0);
  set_led(1, 0, 0xFF, 0);
  set_led(2, 0, 0, 0xFF);
  set_led(3, 0, 0xFF, 0);
  set_led(4, 0xFF, 0, 0);
  send_ws();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
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

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 105-1;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, COL3_Pin|COL2_Pin|ROW1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|GPIO_PIN_6|ROW3_Pin|ROW2_Pin
                          |ROW8_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, COL1_Pin|ROW4_Pin|ROW7_Pin|ROW5_Pin
                          |ROW6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : COL3_Pin COL2_Pin ROW1_Pin */
  GPIO_InitStruct.Pin = COL3_Pin|COL2_Pin|ROW1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin PA6 ROW3_Pin ROW2_Pin
                           ROW8_Pin */
  GPIO_InitStruct.Pin = LD2_Pin|GPIO_PIN_6|ROW3_Pin|ROW2_Pin
                          |ROW8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : COL1_Pin ROW4_Pin ROW7_Pin ROW5_Pin
                           ROW6_Pin */
  GPIO_InitStruct.Pin = COL1_Pin|ROW4_Pin|ROW7_Pin|ROW5_Pin
                          |ROW6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART2 && rx_byte >= 0x00)
  {
    char tx_buf[10];
    int len = 0x00;
    if(rx_byte != 0xFF){
    	len = snprintf(tx_buf, sizeof(tx_buf), "ACK: %d\r\n", rx_byte);
    } else {
    	len = snprintf(tx_buf, sizeof(tx_buf), "END ACK\r\n");
    }
    		 //    gauge(rx_byte, 0, 30, 97);
    rx_buff[0] = rx_buff[1];
    rx_buff[1] = rx_buff[2];
    rx_buff[2] = rx_buff[3];
    rx_buff[3] = rx_byte;

    if(rx_buff[3] == 0xFF){
    	frame_buff[0] = map_byte(rx_buff[0], 30, 97, 1, 8);
    	frame_buff[1] = map_byte(rx_buff[1], 0, 100, 1, 8);
    	frame_buff[2] = map_byte(rx_buff[2], 0, 100, 1, 8);
    }
    HAL_UART_Transmit(&huart2, (uint8_t *)tx_buf, len, HAL_MAX_DELAY);

    // Re-arm for next byte
    HAL_UART_Receive_IT(&huart2, &rx_byte, 1);
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (htim->Instance == TIM3) {
		HAL_GPIO_WritePin(ROW_PORT[current_row], ROW_PIN[current_row], 1);
		current_row = (current_row + 1) % 8;

		for(int col = 0; col < 3; col++){
			uint8_t curr_buff = frame_buff[col] >> current_row & 1;
			HAL_GPIO_WritePin(COL_PORT[col], COL_PIN[col], curr_buff);
		}
	    HAL_GPIO_WritePin(ROW_PORT[current_row], ROW_PIN[current_row], 0);
	}
}

void gauge(const uint8_t val, const uint8_t col, const uint8_t level_min,  const uint8_t level_max){
  HAL_GPIO_WritePin(COL_PORT[0], ROW_PIN[0], 1);
  HAL_GPIO_WritePin(COL_PORT[0], ROW_PIN[0], 1);
  HAL_GPIO_WritePin(COL_PORT[0], ROW_PIN[0], 1);

  uint8_t level = map(val, level_min, level_max, 0, 8);
  for(uint8_t i = 0; i < 8; i++){
	  if(i < level){
		  HAL_GPIO_WritePin(ROW_PORT[i], ROW_PIN[i], 0);
	  }
	  else{
		  HAL_GPIO_WritePin(ROW_PORT[i], ROW_PIN[i], 1);
	  }
  }
}

static inline uint8_t map(uint8_t value, uint8_t in_min, uint8_t in_max, uint8_t out_min, uint8_t out_max){
	return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

static inline uint8_t map_byte(uint8_t value, uint8_t in_min, uint8_t in_max, uint8_t out_min, uint8_t out_max){
	return (1 << map(value, in_min, in_max, out_min, out_max)) - 1;
}

void send_ws(){
	uint32_t idx = 0;
	uint32_t data;

	for (int i = 0; i < LED_NUM; i++) {
		data = (LED_Data[i][1] << 16) | (LED_Data[i][0] << 8) | LED_Data[i][2];

		for (int n = 23; n >= 0; n--) {
			if(data & (1 << n)) pwm_data[idx] = 71; // 2/3 105
			else pwm_data[idx] = 34; // 1/3 105
			idx++;
		}
	}

	for (int p = 0; p < 50; p++) {
		pwm_data[idx] = 0;
		idx++;
	}

	HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_1, (uint32_t *)pwm_data, PWM_DATA_SIZE);
	while(!led_data_sent) {};
	led_data_sent = 0;
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){
	HAL_TIM_PWM_Stop_DMA(&htim4, TIM_CHANNEL_1);
	led_data_sent = 1;
}

void set_led(const uint8_t led_index, const uint8_t r, const uint8_t g, const uint8_t b){
	LED_Data[led_index][0] = r;
	LED_Data[led_index][1] = g;
	LED_Data[led_index][2] = b;
}

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
#ifdef USE_FULL_ASSERT
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

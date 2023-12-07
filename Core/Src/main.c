/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "fatfs.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_port_fs.h"
#include "lv_ex_conf.h"
#include "lv_obj_style_dec.h"
#include "lv_conf.h"
#include "lv_demo_widgets.h"
#include "stdio.h"
#include "touch.h"
#include "lv_demo_stress.h"
#include "lv_app.h"
#include "wlan.h"
#include "sd.h"
#include "Fire.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
///////////////////////////////ESP/////////////////////////////
uint8_t test_sign=1;
///////////////////////////////ADC/////////////////////////////
uint16_t adcData[10]={0};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/////////////////////////////////////    LVGL_时钟    ///////////////////////////////////
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == (&htim4))
    {
        lv_tick_inc(1);
    }
}
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
  MX_ADC1_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_SPI3_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
/////////////////////////////////////    TIM1_ESP    ///////////////////////////////////
//	HAL_TIM_Base_Start(&htim1);	
/////////////////////////////////////    TIM2_PWM    ///////////////////////////////////
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
/////////////////////////////////////    TIM3_Vaalue   ///////////////////////////////////
	HAL_TIM_Base_Start(&htim3);	
/////////////////////////////////////    TIM4_LVGL   ///////////////////////////////////
	HAL_TIM_Base_Start_IT(&htim4);	
/////////////////////////////////////    TIM5_Fire   ///////////////////////////////////
//	HAL_TIM_Base_Start(&htim5);
/////////////////////////////////////    ADC1_Fire   ///////////////////////////////////
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)adcData,10);
/////////////////////////////////////    SD_Init     ///////////////////////////////////
	if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9)==GPIO_PIN_RESET&&sd_init_sign==0)		SD_Init();
	else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9)==GPIO_PIN_SET)									sd_init_sign=0;
/////////////////////////////////////    LCD_Init    ///////////////////////////////////
	LCD_Init();
/////////////////////////////////////    TOUCH_Init  ///////////////////////////////////	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET); //CS
	send_16way(0x80);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
/////////////////////////////////////   LVGL_Init   ////////////////////////////////////	
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();
	lv_port_fs_init();
	FATFS ffs;
	FRESULT fres = FR_NOT_READY;
	fres = f_mount(&ffs, "0:/", 0);
	lv_demo_myself();
	
//	lv_demo_widgets();
//	lv_demo_stress();
	
//	lv_fs_file_t lv_file;
//  lv_fs_res_t  lv_res;
//	lv_res = lv_fs_open( &lv_file, "0:/test2.txt", LV_FS_MODE_RD );
//	if ( lv_res != LV_FS_RES_OK ) {
//		char string[10];
//		sprintf(string,"LVGL FS open error. (%d)\n", lv_res);
//		lv_textarea_add_text(ta1, string);
//	} else {
//		char string[10];
//		sprintf(string,"LVGL FS open Ok\n");
//		lv_textarea_add_text(ta1, string);
//	}
//	lv_fs_close(&lv_file);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		CS();
		lv_task_handler();
		ESP_Init();
		
		if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9)==GPIO_PIN_RESET&&sd_init_sign==0)		SD_Init();
		else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9)==GPIO_PIN_SET)									sd_init_sign=0;
		
		CS();
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		if(recv_end_flag == 1)  //接收完成标志
		{
			static char string[200];
			static char *ready="ready";
			static uint8_t add_sign=0;
			sprintf(string,"%s",rx_buffer);
			
			if(test_sign==1)
			{
				if(strstr(string,ready)!=NULL) 	add_sign=1;
				else 	add_sign=0;
			}
			
			if(add_sign==1)
			{
				control_test_0(string);
//				control_test_1(string);
				lv_textarea_add_text(ta1, string);
				test_sign=0;
			}
			
			memset(rx_buffer,0,rx_len);
			memset(string,'\0',rx_len);
			rx_len = 0;//清除计数
			recv_end_flag = 0;//清除接收结束标志
    }
		HAL_UART_Receive_DMA(&huart1,rx_buffer,BUFFER_SIZE);//重新打开DMA接收
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
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

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
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include <oled.h>
#include "hx711.h"
#include "aht20.h"
#include <oled1.h>
#include "math.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

float t;                           //温度，过高则代表火警
int floor2_now;                     //2号电梯现在所在的楼层
int floor2_will;                    //2号电梯将要到达的楼层
int floor1_now;                     //1号电梯现在所在的楼层
int floor1_will;                    //1号电梯将要到达的楼层
float weight_1;
float weight_2;

HX711 data = {                      //hx711信息2
		.gpioSck = GPIOC, .gpioData = GPIOC, .pinSck = GPIO_PIN_14, .pinData =
		GPIO_PIN_13, .offset = 0, .gain = 1 };
HX711 data1 = { .gpioSck = GPIOB, .gpioData = GPIOB, .pinSck = GPIO_PIN_9,
		.pinData =
		GPIO_PIN_8, .offset = 0, .gain = 1 };

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
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	HAL_Delay(50);
	OLED_Init();
	OLED1_Init();

	HAL_Delay(50);
	AHT20_Init();                 //温湿度传感器使用i2c1
	HAL_Delay(50);

	HX711_Init(data);
	HAL_Delay(100);

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_ALL);

//	int floor1;                         //电梯1实际楼层
//	int floor2;                         //电梯2实际楼层
//	int temp;                           //环境温度，用于显示火警
//	int weight1;                        //电梯1载重
//	int weight2;                        //电梯2载重
	char message1_f[24] = "";             //电梯1中显示屏内容1_floor
	char message2_f[24] = "";           //电梯2中显示屏内容2_floor
	char message1_w[24] = "";
	char message2_w[24] = "";
//	double t;                           //温度，过高则代表火警
//	typedef enum {                       //电梯运行状态
//		up, down, still
//	} state;

	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 50);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 50);
	OLED_NewFrame();
	OLED1_NewFrame();
	OLED_PrintString(0, 0, "1楼", &font16x16, OLED_COLOR_NORMAL);
	OLED1_PrintString(0, 0, "1楼", &font16x16, OLED_COLOR_NORMAL);
	OLED_ShowFrame();
	OLED1_ShowFrame();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {

		t = AHT20_Temp();
		if (t >= 25) {
			HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3); // 这里就近停下的功能在使用舵机的情况下似乎有点难以实现，先用直接停止代替。。。
			HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);   //光报警

			OLED_NewFrame();
			OLED_PrintString(0, 0, "火警", &font16x16, OLED_COLOR_NORMAL);
			OLED_ShowFrame();
			OLED1_NewFrame();
			OLED1_PrintString(0, 0, "火警", &font16x16, OLED_COLOR_NORMAL);
			OLED1_ShowFrame();

			HAL_NVIC_DisableIRQ(EXTI0_IRQn);      // 禁用 EXTI Line0 中断
			HAL_NVIC_DisableIRQ(EXTI1_IRQn);      // 禁用 EXTI Line1 中断
			HAL_NVIC_DisableIRQ(EXTI2_IRQn);      // 禁用 EXTI Line2 中断
			HAL_NVIC_DisableIRQ(EXTI3_IRQn);      // 禁用 EXTI Line3 中断
			HAL_NVIC_DisableIRQ(EXTI4_IRQn);      // 禁用 EXTI Line4 中断
			HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);    // 禁用 EXTI Line5-9 中断
			HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);  // 禁用 EXTI Line10-15 中断
		} else {
			HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
			HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
			OLED_NewFrame();

			HAL_NVIC_EnableIRQ(EXTI0_IRQn);      // 启用 EXTI Line0 中断
			HAL_NVIC_EnableIRQ(EXTI1_IRQn);      // 启用 EXTI Line1 中断
			HAL_NVIC_EnableIRQ(EXTI2_IRQn);      // 启用 EXTI Line2 中断
			HAL_NVIC_EnableIRQ(EXTI3_IRQn);      // 启用 EXTI Line3 中断
			HAL_NVIC_EnableIRQ(EXTI4_IRQn);      // 启用 EXTI Line4 中断
			HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);    // 启用 EXTI Line5-9 中断
			HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);  // 启用 EXTI Line10-15 中断


		weight_2 = HX711_getweight(data, 10, 353, 8300000);
		weight_1 = HX711_getweight1(data1, 10, 352, 8328440);
		if (weight_1 > 2000) {
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, SET);
			OLED1_NewFrame();
			OLED1_PrintString(0, 0, "超", &font16x16, OLED_COLOR_NORMAL);
			OLED_PrintString(16, 0, "重", &font16x16, OLED_COLOR_NORMAL);
			OLED1_ShowFrame();
			HAL_Delay(1000);
		} else {
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, RESET);
			OLED1_NewFrame();
		}
		if (weight_2 > 2000) {
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, SET);
			OLED_NewFrame();
			OLED_PrintString(0, 0, "超重", &font16x16, OLED_COLOR_NORMAL);
			OLED_ShowFrame();
			HAL_Delay(1000);
		} else {
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, RESET);
			OLED_NewFrame();
		}

		//第二部电梯控制程序

		floor2_now = (__HAL_TIM_GET_COMPARE(&htim3,TIM_CHANNEL_4) - 10) / 40; //电梯将要到达的楼层
		//floor2_now = floor2_will;                                  //显示电梯实际到达的楼层
		floor1_now = (__HAL_TIM_GET_COMPARE(&htim3,TIM_CHANNEL_3) - 10) / 40;
		sprintf(message1_f, "%d", floor1_now);
		sprintf(message2_f, "%d", floor2_now);
		sprintf(message1_w, "%.1f", weight_1);
		sprintf(message2_w, "%.1f", weight_2);
		OLED_NewFrame();
		OLED1_NewFrame();
		OLED_PrintString(0, 0, message2_f, &font16x16, OLED_COLOR_NORMAL);
		OLED_PrintString(17, 0, "楼", &font16x16, OLED_COLOR_NORMAL);
		OLED_PrintString(0, 17, "质量", &font16x16, OLED_COLOR_NORMAL);
		OLED_PrintString(41, 17, message2_w, &font16x16, OLED_COLOR_NORMAL);
		OLED1_PrintString(0, 0, message1_f, &font16x16, OLED_COLOR_NORMAL);
		OLED1_PrintString(17, 0, "楼", &font16x16, OLED_COLOR_NORMAL);
		OLED1_PrintString(0, 17, "质量", &font16x16, OLED_COLOR_NORMAL);
		OLED1_PrintString(41, 17, message1_w, &font16x16, OLED_COLOR_NORMAL);
		OLED_ShowFrame();
		OLED1_ShowFrame();
		}
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
	while (1) {
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

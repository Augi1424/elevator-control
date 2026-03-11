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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "hx711.h"
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
#define hx711_2data_Pin GPIO_PIN_13
#define hx711_2data_GPIO_Port GPIOC
#define hx711_2sck_Pin GPIO_PIN_14
#define hx711_2sck_GPIO_Port GPIOC
#define tare_Pin GPIO_PIN_15
#define tare_GPIO_Port GPIOC
#define tare_EXTI_IRQn EXTI15_10_IRQn
#define fire_alarm_Pin GPIO_PIN_12
#define fire_alarm_GPIO_Port GPIOB
#define overload_alarm_1_Pin GPIO_PIN_13
#define overload_alarm_1_GPIO_Port GPIOB
#define overload_alarm_2_Pin GPIO_PIN_14
#define overload_alarm_2_GPIO_Port GPIOB
#define elevator_reset_Pin GPIO_PIN_3
#define elevator_reset_GPIO_Port GPIOB
#define elevator_reset_EXTI_IRQn EXTI3_IRQn
#define hx711_1data_Pin GPIO_PIN_8
#define hx711_1data_GPIO_Port GPIOB
#define hx711_1sck_Pin GPIO_PIN_9
#define hx711_1sck_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
extern float t;
extern float weight_1;
extern float weight_2;
extern int floor2_now;               //2号电梯现在所在的楼层
extern int floor2_will;              //2号电梯将要到达的楼层
extern int floor1_now;               //1号电梯现在所在的楼层
extern int floor1_will;              //1号电梯将要到达的楼层
extern HX711 data;
extern HX711 data1;
//typedef enum{                        //oled屏显示的界面。mainscreen为主界面，datascreen为数据界面
//	mainscreen,datascreen
//}display;
//extern display;
//
//typedef enum {                       //电梯运行状态
//	up, down, still
//} state;
//extern state;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

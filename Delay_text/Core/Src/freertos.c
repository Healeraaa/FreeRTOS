/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
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
/* USER CODE BEGIN Variables */
TaskHandle_t LEDToggle_Task_Handler;
TaskHandle_t SerialSend_Task_Handler;

/* USER CODE END Variables */
/* Definitions for StartTask */
osThreadId_t StartTaskHandle;
const osThreadAttr_t StartTask_attributes = {
    .name = "StartTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void LEDToggle_Task(void *argument)
{
  while (1)
  {
    LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
    osDelay(500);
  }
}

void SerialSend_Task(void *argument)
{
  while (1)
  {
    printf("Hello World!\r\n");
    if (LL_USART_ReceiveData8(USART1) == 0x55)
    {
      if (LEDToggle_Task_Handler != NULL)
      {
        printf("删除LEDToggle_Task");
        vTaskDelete(LEDToggle_Task_Handler);
        LEDToggle_Task_Handler = NULL;
      }
    }
    osDelay(1000);
  }
}

/* USER CODE END FunctionPrototypes */

void start_task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of StartTask */
  StartTaskHandle = osThreadNew(start_task, NULL, &StartTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */

  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */
}

/* USER CODE BEGIN Header_start_task */
/**
 * @brief  Function implementing the StartTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_start_task */
void start_task(void *argument)
{
  /* USER CODE BEGIN start_task */
  /* Infinite loop */
  xTaskCreate(
      (TaskFunction_t)LEDToggle_Task,
      (const char *const)"LEDToggle_Task",
      (const configSTACK_DEPTH_TYPE)128,
      (void *)NULL,
      (UBaseType_t)osPriorityNormal1,
      (TaskHandle_t *)&LEDToggle_Task_Handler);
  xTaskCreate(
      (TaskFunction_t)SerialSend_Task,
      (const char *const)"SerialSend_Task",
      (const configSTACK_DEPTH_TYPE)128,
      (void *)NULL,
      (UBaseType_t)osPriorityNormal1,
      (TaskHandle_t *)&SerialSend_Task_Handler);

  vTaskDelete(NULL);

  /* USER CODE END start_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

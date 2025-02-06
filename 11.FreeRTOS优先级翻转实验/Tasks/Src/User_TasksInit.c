/* Private includes -----------------------------------------------------------*/
#include "user_TasksInit.h"
#include "AToD.h"
#include "stdio.h"
#include "OLED.h"
#include "key.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Timers --------------------------------------------------------------------*/

/* Tasks ---------------------------------------------------------------------*/
TaskHandle_t KeyScanf_Task_Handler;
TaskHandle_t LEDToggle_Task_Handler;
TaskHandle_t SerialSend_Task_Handler;

/* Queues --------------------------------------------------------------------*/
QueueHandle_t semphore_handle;
/**
 * @brief  按键读取任务
 * @param  argument: Not used
 * @retval None
 */

void KeyScanf_Task(void *argument)
{
  /* Low Task */
      while(1) 
    {
        printf("low_task获取信号量\r\n");
        xSemaphoreTake(semphore_handle,portMAX_DELAY);
        printf("low_task正在运行！！！\r\n");
        delay_ms(3000);
        printf("low_task释放信号量\r\n");
        xSemaphoreGive(semphore_handle); 
        vTaskDelay(1000);
    }
}

/**
 * @brief  LED间隔0.5s闪烁一次
 * @param  argument: Not used
 * @retval None
 */
void LEDToggle_Task(void *argument)
{
  /* Middle Task */
    while(1)
    {
        printf("middle_task正在运行！！！\r\n");
        vTaskDelay(1000);
    }
}
/**
 * @brief  UART串口打印进行数据显示
 * @param  argument: Not used
 * @retval None
 */
void SerialSend_Task(void *argument)
{
  /* Heigh Task */
    while(1)
    {
        printf("high_task获取信号量\r\n");
        xSemaphoreTake(semphore_handle,portMAX_DELAY);
        printf("high_task正在运行！！！\r\n");
        delay_ms(1000);
        printf("high_task释放信号量\r\n");
        xSemaphoreGive(semphore_handle); 
        vTaskDelay(1000);
    }
}

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void User_Tasks_Init(void)
{
    semphore_handle = xSemaphoreCreateBinary();
    if(semphore_handle != NULL)
    {
        printf("二值信号量创建成功！！！\r\n");
    }
    xSemaphoreGive(semphore_handle);        /* 释放一次信号量 */

  xTaskCreate(KeyScanf_Task, "KeyScanf_Task", 128, NULL, 1, &KeyScanf_Task_Handler);
  xTaskCreate(LEDToggle_Task, "LEDToggle_Task", 128, NULL, 2, &LEDToggle_Task_Handler);
  xTaskCreate(SerialSend_Task, "SerialSend_Task", 128, NULL, 3, &SerialSend_Task_Handler);
}

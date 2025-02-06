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
QueueHandle_t count_semaphore_handle;
/**
 * @brief  按键读取任务
 * @param  argument: Not used
 * @retval None
 */

void KeyScanf_Task(void *argument)
{
  /* 释放计数型信号量 */
  uint8_t KeyNum = 0;
  BaseType_t err = 0;
  while (1)
  {
    KeyNum = Key_GetNum(0);
    if (KeyNum == 1)
    {
      if (count_semaphore_handle != NULL)
      {
        err = xSemaphoreGive(count_semaphore_handle);
        if (err == pdPASS)
          printf("计数型信号量释放成功\r\n");
        else
          printf("计数型信号量释放失败\r\n");
      }
    }
    vTaskDelay(20);
  }
}

/**
 * @brief  LED间隔0.5s闪烁一次
 * @param  argument: Not used
 * @retval None
 */
void LEDToggle_Task(void *argument)
{
  while (1)
  {
    LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
    vTaskDelay(500);
  }
}
/**
 * @brief  UART串口打印进行数据显示
 * @param  argument: Not used
 * @retval None
 */
void SerialSend_Task(void *argument)
{
  /* 获取计数型信号量 */
  BaseType_t err = 0;
  UBaseType_t count_num = 0;
  while (1)
  {
    err = xSemaphoreTake(count_semaphore_handle, 0);
    if (err == pdPASS)
    {
      count_num = uxSemaphoreGetCount(count_semaphore_handle);
      printf("计数型信号量获取成功\r\n");
      printf("计数型信号量的值为%ld\r\n",count_num);
    }
    else
      printf("计数型信号量获取失败\r\n");

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
  count_semaphore_handle = xSemaphoreCreateCounting(100, 50);
  if (count_semaphore_handle == NULL)
    printf("计数型信号量创建失败\r\n");
  else
    printf("计数型信号量创建成功\r\n");

  xTaskCreate(KeyScanf_Task, "KeyScanf_Task", 128, NULL, 3, &KeyScanf_Task_Handler);
  xTaskCreate(LEDToggle_Task, "LEDToggle_Task", 128, NULL, 1, &LEDToggle_Task_Handler);
  xTaskCreate(SerialSend_Task, "SerialSend_Task", 128, NULL, 2, &SerialSend_Task_Handler);
}

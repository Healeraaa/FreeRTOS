/* Private includes -----------------------------------------------------------*/
#include "user_TasksInit.h"
#include "AToD.h"
#include "stdio.h"
#include "OLED.h"
#include "key.h"
#include "LED.h"

/* Private typedef -----------------------------------------------------------*/
#define EVENT_BIT1 (1 << 1)
#define EVENT_BIT2 (1 << 2)
/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Timers --------------------------------------------------------------------*/
void Timer1_Callback(TimerHandle_t xTimer);
void Timer2_Callback(TimerHandle_t xTimer);

TimerHandle_t Time1_handle; // 单次定时器
TimerHandle_t Time2_handle; // 循环定时器
/* Tasks ---------------------------------------------------------------------*/
TaskHandle_t KeyScanf_Task_Handler;
TaskHandle_t LEDToggle_Task_Handler;
TaskHandle_t SerialSend_Task_Handler;

/* Queues --------------------------------------------------------------------*/

/* Events --------------------------------------------------------------------*/
EventGroupHandle_t eventgrount_handle;

/**
 * @brief  按键读取任务
 * @param  argument: Not used
 * @retval None
 */

void KeyScanf_Task(void *argument)
{
  /* 按键扫描并控制软件定时器 */
  uint8_t KeyNum = 0;
  while (1)
  {
    KeyNum = Key_GetNum(0);
    if(KeyNum == KEY1_PRES)
    {
      xTimerStart(Time1_handle,portMAX_DELAY);
      xTimerStart(Time2_handle,portMAX_DELAY);
    }
    else if(KeyNum == KEY2_PRES)
    {
      xTimerStop(Time1_handle,portMAX_DELAY);
      xTimerStop(Time2_handle,portMAX_DELAY);
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
    LED_Reveral();
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
  while (1)
  {
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
  /* 单次定时器 */
  Time1_handle = xTimerCreate("Timer1", 1000, pdFALSE, (void *)1, Timer1_Callback);
  /* 周期定时器 */
  Time2_handle = xTimerCreate("Timer2", 1000, pdTRUE, (void *)2, Timer2_Callback);

  xTaskCreate(KeyScanf_Task, "KeyScanf_Task", 128, NULL, 2, &KeyScanf_Task_Handler);
  xTaskCreate(LEDToggle_Task, "LEDToggle_Task", 128, NULL, 1, &LEDToggle_Task_Handler);
  xTaskCreate(SerialSend_Task, "SerialSend_Task", 128, NULL, 3, &SerialSend_Task_Handler);
}

void Timer1_Callback(TimerHandle_t xTimer)
{
  static uint32_t timer = 0;
  printf("Tim1:%d\r\n", ++timer);
}

void Timer2_Callback(TimerHandle_t xTimer)
{
  static uint32_t timer = 0;
  printf("Tim2:%d\r\n", ++timer);
}
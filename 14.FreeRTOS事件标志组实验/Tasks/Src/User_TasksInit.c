/* Private includes -----------------------------------------------------------*/
#include "user_TasksInit.h"
#include "AToD.h"
#include "stdio.h"
#include "OLED.h"
#include "key.h"

/* Private typedef -----------------------------------------------------------*/
#define EVENTBIT_0 (1 << 0)
#define EVENTBIT_1 (1 << 1)
/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Timers --------------------------------------------------------------------*/

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
  uint8_t KeyNum = 0;
  while (1)
  {
    KeyNum = Key_GetNum(0);
    if (KeyNum == 2)
    {
      xEventGroupSetBits(eventgrount_handle, EVENTBIT_0);
    }
    else if (KeyNum == 3)
    {
      xEventGroupSetBits(eventgrount_handle, EVENTBIT_1);
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
  EventBits_t event_bit = 0;
  while (1)
  {
    event_bit = xEventGroupWaitBits(eventgrount_handle, EVENTBIT_0 | EVENTBIT_1, pdTRUE,pdFALSE, portMAX_DELAY);
    printf("等到的事件标志位值为：%#x\r\n", event_bit);
    // vTaskDelay(20);
  }
}

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void User_Tasks_Init(void)
{
  eventgrount_handle = xEventGroupCreate();
  if (eventgrount_handle == NULL)
    printf("事件标志组创建失败！！！\r\n");
  else
    printf("事件标志组创建成功！！！\r\n");

  xTaskCreate(KeyScanf_Task, "KeyScanf_Task", 128, NULL, 2, &KeyScanf_Task_Handler);
  xTaskCreate(LEDToggle_Task, "LEDToggle_Task", 128, NULL, 1, &LEDToggle_Task_Handler);
  xTaskCreate(SerialSend_Task, "SerialSend_Task", 128, NULL, 3, &SerialSend_Task_Handler);
}

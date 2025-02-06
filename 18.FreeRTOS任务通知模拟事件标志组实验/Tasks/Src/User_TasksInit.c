/* Private includes -----------------------------------------------------------*/
#include "user_TasksInit.h"
#include "AToD.h"
#include "stdio.h"
#include "OLED.h"
#include "key.h"

/* Private typedef -----------------------------------------------------------*/
#define EVENT_BIT1 (1 << 1)
#define EVENT_BIT2 (1 << 2)
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
  /* 发送任务通知值 */
  uint8_t KeyNum = 0;
  while (1)
  {
    KeyNum = Key_GetNum(0);
    if ((KeyNum == KEY1_PRES || KeyNum == KEY2_PRES) && SerialSend_Task_Handler != NULL)
    {
      printf("更新BIT%d位\r\n", KeyNum);
      xTaskNotify(SerialSend_Task_Handler, 1 << KeyNum, eSetBits);
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
  BaseType_t err;
  uint32_t notify_val = 0, event_bit = 0;
  while (1)
  {
    err = xTaskNotifyWait(0x00, 0xFFFFFFFF, &notify_val, portMAX_DELAY);
    if (err != 0)
    {
      if (notify_val & EVENT_BIT1)
        event_bit |= EVENT_BIT1;
      if (notify_val & EVENT_BIT2)
        event_bit |= EVENT_BIT2;
      if (event_bit == (EVENT_BIT1 | EVENT_BIT2))
      {
        event_bit = 0;
        printf("任务通知模拟事件标志组接收成功！！！\r\n");
      }
    }
  }
}

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void User_Tasks_Init(void)
{

  xTaskCreate(KeyScanf_Task, "KeyScanf_Task", 128, NULL, 2, &KeyScanf_Task_Handler);
  xTaskCreate(LEDToggle_Task, "LEDToggle_Task", 128, NULL, 1, &LEDToggle_Task_Handler);
  xTaskCreate(SerialSend_Task, "SerialSend_Task", 128, NULL, 3, &SerialSend_Task_Handler);
}

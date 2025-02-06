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
    if ((KeyNum == KEY1_PRES || KeyNum == KEY2_PRES || KeyNum == KEY3_PRES || KeyNum == KEY4_PRES) && SerialSend_Task_Handler != NULL)
    {
      printf("任务通知模拟消息邮箱发送，发送的键值是：%d\r\n", KeyNum);
      xTaskNotify(SerialSend_Task_Handler, KeyNum, eSetValueWithOverwrite);
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
  uint32_t num = 0;
  while (1)
  {
    err = xTaskNotifyWait(0x00, 0xFFFFFFFF, &num, portMAX_DELAY);
    if (err != 0)
    {
      printf("接收的值为：%ld\r\n", num);
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

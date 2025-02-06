/* Private includes -----------------------------------------------------------*/
#include "user_TasksInit.h"
#include "AToD.h"
#include "stdio.h"
#include "OLED.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Timers --------------------------------------------------------------------*/

/* Tasks ---------------------------------------------------------------------*/
TaskHandle_t LEDToggle_Task_Handler;
TaskHandle_t SerialSend_Task_Handler;

List_t TestList;
ListItem_t ListItem1;
ListItem_t ListItem2;
ListItem_t ListItem3;

/**
 * @brief  LED间隔0.5s闪烁一次
 * @param  argument: Not used
 * @retval None
 */
void LEDToggle_Task(void *argument)
{
  uint8_t num = 0;
  while (1)
  {
    LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
    osDelay(500);
  }
}
/**
 * @brief  UART串口打印进行数据显示
 * @param  argument: Not used
 * @retval None
 */
void SerialSend_Task(void *argument)
{
  
}

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void User_Tasks_Init(void)
{
  xTaskCreate(LEDToggle_Task, "LEDToggle_Task", 128, NULL, osPriorityNormal, &LEDToggle_Task_Handler);
  xTaskCreate(SerialSend_Task, "SerialSend_Task", 128, NULL, osPriorityNormal, &SerialSend_Task_Handler);
}

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
QueueHandle_t key_queue_handler;
QueueHandle_t big_data_queue_handler;
char buff[100] = {"我是一个大数组，超级大的数组 1234 1234 像首歌 huiqwdgwqigyq"};

/**
 * @brief  按键读取任务
 * @param  argument: Not used
 * @retval None
 */

void KeyScanf_Task(void *argument)
{
  uint8_t KeyNum = 0;
  char *buf;
  BaseType_t err = 0;
  buf = buff; // 等效buf = &buff[0]
  while (1)
  {
    KeyNum = Key_GetNum(0);
    if (KeyNum == 2 || KeyNum == 3)
    {
      err = xQueueSend(key_queue_handler, &KeyNum, portMAX_DELAY);
      if (err == pdTRUE)
        printf("key_queue写入成功！！！\r\n");
      else
        printf("key_queue写入失败！！！\r\n");
    }
    else if (KeyNum == 4)
    {
      err = xQueueSend(big_data_queue_handler, &buf, portMAX_DELAY);
      if (err == pdTRUE)
        printf("big_data_queue写入成功！！！\r\n");
      else
        printf("big_data_queue写入失败！！！\r\n");
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
  /* 小数据出队 */
  uint8_t num = 0;
  BaseType_t err = 0;
  while (1)
  {
    err = xQueueReceive(key_queue_handler, &num, portMAX_DELAY);
    if (err == pdTRUE)
      printf("key_queue读取数据为%d！！！\r\n", num);
    else
      printf("key_queue读取失败！！！\r\n");
    vTaskDelay(20);

    // LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
    // vTaskDelay(500);
  }
}
/**
 * @brief  UART串口打印进行数据显示
 * @param  argument: Not used
 * @retval None
 */
void SerialSend_Task(void *argument)
{
  /* 大数据出队 */
  BaseType_t err = 0;
  char *buf;
  while (1)
  {
    err = xQueueReceive(big_data_queue_handler, &buf, portMAX_DELAY);
    if (err == pdTRUE)
      printf("big_data_queue_handler读取数据为%s！！！\r\n", buf);
    else
      printf("big_data_queue_handler读取失败！！！\r\n");
    vTaskDelay(20);
  }
}

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void User_Tasks_Init(void)
{
  key_queue_handler = xQueueCreate(2, sizeof(uint8_t));
  if (key_queue_handler != NULL)
    printf("key_queue创建成功！！！\r\n");
  else
    printf("key_queue创建失败！！！\r\n");

  big_data_queue_handler = xQueueCreate(1, sizeof(char *));
  if (key_queue_handler != NULL)
    printf("big_data_queue创建成功！！！\r\n");
  else
    printf("big_data_queue创建失败！！！\r\n");

  xTaskCreate(KeyScanf_Task, "KeyScanf_Task", 128, NULL, 3, &KeyScanf_Task_Handler);
  xTaskCreate(LEDToggle_Task, "LEDToggle_Task", 128, NULL, 2, &LEDToggle_Task_Handler);
  xTaskCreate(SerialSend_Task, "SerialSend_Task", 128, NULL, 2, &SerialSend_Task_Handler);
}

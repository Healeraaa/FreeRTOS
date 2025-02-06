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
QueueHandle_t set_queue_handler;
QueueHandle_t queue_handler;
QueueHandle_t set_queue_handler;
QueueHandle_t semaphor_handler;

/**
 * @brief  按键读取任务
 * @param  argument: Not used
 * @retval None
 */

void KeyScanf_Task(void *argument)
{
  uint8_t KeyNum = 0;
  BaseType_t err = 0;
  while (1)
  {
    KeyNum = Key_GetNum(0);
    if(KeyNum == 2)
    {
      err = xQueueSend(queue_handler,&KeyNum,portMAX_DELAY);
      if(err == pdPASS)
        printf("往队列queue写入数据成功\r\n");
    }
    if(KeyNum == 3)
    {
      err = xSemaphoreGive(semaphor_handler);
      if(err == pdPASS)
        printf("semaphor释放成功\r\n");
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
  QueueSetMemberHandle_t setmember_handle;
  BaseType_t err = 0;
  uint8_t num;
  while (1)
  {
    setmember_handle = xQueueSelectFromSet(set_queue_handler,portMAX_DELAY);
    if(setmember_handle == queue_handler)
    {
      xQueueReceive(queue_handler,&num,portMAX_DELAY);
      printf("队列queue接收到数据:%d\r\n",num);
    }
    else if(setmember_handle == semaphor_handler)
    {
      xSemaphoreTake(semaphor_handler,portMAX_DELAY);
      printf("信号量接收数据\r\n");
    }
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
  /* 创建队列集 */
  set_queue_handler = xQueueCreateSet(2);
  if (set_queue_handler == NULL)
    printf("set_queue创建失败！\r\n");
  else
    printf("set_queue创建成功！\r\n");

  /* 创建队列 */
  queue_handler = xQueueCreate(1, sizeof(uint8_t));
  if (queue_handler == NULL)
    printf("queue创建失败！\r\n");
  else
    printf("queue创建成功！\r\n");

  /* 创建信号量 */
  vSemaphoreCreateBinary(semaphor_handler);
  if (semaphor_handler == NULL)
    printf("semaphor创建失败！\r\n");
  else
    printf("semaphor创建成功！\r\n");
  xSemaphoreTake(semaphor_handler,portMAX_DELAY);//获取一次信号量

  xQueueAddToSet(queue_handler,set_queue_handler);//添加队列到队列集中
  xQueueAddToSet(semaphor_handler,set_queue_handler);//添加信号量到队列集中

  xTaskCreate(KeyScanf_Task, "KeyScanf_Task", 128, NULL, 2, &KeyScanf_Task_Handler);
  xTaskCreate(LEDToggle_Task, "LEDToggle_Task", 128, NULL, 1, &LEDToggle_Task_Handler);
  xTaskCreate(SerialSend_Task, "SerialSend_Task", 128, NULL, 3, &SerialSend_Task_Handler);
}

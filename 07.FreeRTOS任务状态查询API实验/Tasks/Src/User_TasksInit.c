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
  // UBaseType_t TaskPriority_num = 0;//任务优先级
  // UBaseType_t TaskNun = 0;//任务数量
  // TaskPriority_num = uxTaskPriorityGet(LEDToggle_Task_Handler);
  // printf("The first Task Priority is %d\r\n",TaskPriority_num);
  // vTaskPrioritySet(LEDToggle_Task_Handler,TaskPriority_num-1);
  // TaskPriority_num = uxTaskPriorityGet(LEDToggle_Task_Handler);
  // printf("The second Task Priority is %d\r\n",TaskPriority_num);
  // TaskNun = uxTaskGetNumberOfTasks();
  // printf("系统任务数量为 %d\r\n",TaskNun);//包含用户创建任务 + 空闲任务 + 软件定时器任务

  // /* 获取所有任务的状态信息 */
  //   TaskStatus_t *pxTaskStatusArray;
  //   volatile UBaseType_t uxArraySize, x;
  //   uxArraySize = uxTaskGetNumberOfTasks();
  //   pxTaskStatusArray = pvPortMalloc(uxArraySize * sizeof(TaskStatus_t));
  //   uxArraySize = uxTaskGetSystemState(pxTaskStatusArray,
  //                                      uxArraySize,
  //                                      NULL);
  //   printf("任务名\t\t\t任务优先级\t\t任务编号\r\n");
  //   for (x = 0; x < uxArraySize; x++)
  //   {
  //     printf("%s\t\t\t\t%ld\t\t%ld\r\n",
  //            pxTaskStatusArray[x].pcTaskName,
  //            pxTaskStatusArray[x].uxCurrentPriority,
  //            pxTaskStatusArray[x].xTaskNumber);
  //   }

  // /* 获取单个任务的状态信息 */
  // TaskStatus_t *pxTaskStatus;
  // pxTaskStatus = pvPortMalloc(sizeof(TaskStatus_t));
  // vTaskGetInfo(LEDToggle_Task_Handler,
  //              pxTaskStatus,
  //              pdTRUE,
  //              eInvalid);
  // printf("任务名：%s\r\n",pxTaskStatus->pcTaskName);
  // printf("任务优先级：%ld\r\n",pxTaskStatus->uxCurrentPriority);
  // printf("任务编号：%ld\r\n",pxTaskStatus->xTaskNumber);
  // printf("任务状态：%ld\r\n",pxTaskStatus->eCurrentState);

  // /* 通过任务名获取任务句柄 */
  // TaskHandle_t task_handler;
  // task_handler = xTaskGetHandle("LEDToggle_Task");
  // printf("获取的任务句柄：%#x\r\n",(int)task_handler);
  // printf("LED闪烁的任务句柄：%#x\r\n",(int)LEDToggle_Task_Handler);

  // /* 获取指定任务的任务栈历史最小剩余堆栈 */
  // UBaseType_t task_stack_min = 0;
  // task_stack_min = uxTaskGetStackHighWaterMark(LEDToggle_Task_Handler);
  // printf("LED闪烁任务历史最小堆栈：%ld\r\n",task_stack_min);

  // /* 查询某个任务的运行状态 */
  // eTaskState task_state[2] = {0};
  // task_state[0] = eTaskGetState(LEDToggle_Task_Handler);
  // task_state[1] = eTaskGetState(SerialSend_Task_Handler);
  // printf("LED闪烁任务运行状态：%ld\r\n",task_state[0]);
  // printf("UART发送任务运行状态：%ld\r\n",task_state[1]);

  char task_buff[600];//注意：使用vTaskList 需要给任务分配栈内存，避免溢出
  vTaskList(task_buff);
  printf("%s\r\n",task_buff);

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
  xTaskCreate(LEDToggle_Task, "LEDToggle_Task", 128, NULL, osPriorityNormal, &LEDToggle_Task_Handler);
  xTaskCreate(SerialSend_Task, "SerialSend_Task", 1280, NULL, osPriorityNormal, &SerialSend_Task_Handler);
}

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "User_TasksInit.h"
#include "stdio.h"
#include "key.h"
#include "delay.h"

TaskHandle_t Key_Task_Handler;

void Key_Task(void *argument)
{
  while (1)
  {
   Key_GetNum(0);
    osDelay(1);
  }
}

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
  User_Tasks_Init();
}



 
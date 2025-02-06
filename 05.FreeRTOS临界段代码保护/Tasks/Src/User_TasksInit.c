/* Private includes -----------------------------------------------------------*/
// includes
#include "user_TasksInit.h"
#include "AToD.h"
#include "stdio.h"
#include <stdlib.h>
#include "task.h"
#include "OLED.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Timers --------------------------------------------------------------------*/

/* Tasks ---------------------------------------------------------------------*/

typedef struct
{
  TaskHandle_t taskHandle;
  int executionTime; // 任务的执行时间
  const char *taskName;
} TaskInfo;

// 比较函数，用于按执行时间排序（从小到大）
int compare(const void *a, const void *b)
{
  return ((TaskInfo *)a)->executionTime - ((TaskInfo *)b)->executionTime;
}

// 模拟的任务 A
void TaskA(void *pvParameters)
{
  while (1)
  {
    printf("Task A is running\n");
    vTaskDelay(pdMS_TO_TICKS(1000)); // 模拟任务 A 执行 1 秒
  }
}

// 模拟的任务 B
void TaskB(void *pvParameters)
{
  while (1)
  {
    printf("Task B is running\n");
    vTaskDelay(pdMS_TO_TICKS(2000)); // 模拟任务 B 执行 2 秒
  }
}

// 模拟的任务 C
void TaskC(void *pvParameters)
{
  while (1)
  {
    printf("Task C is running\n");
    vTaskDelay(pdMS_TO_TICKS(500)); // 模拟任务 C 执行 0.5 秒
  }
}

// 自定义调度器（模拟贪心调度，选择执行时间最短的任务）
void customScheduler(TaskInfo *tasks, int taskCount)
{
  // 排序任务列表（按照任务执行时间升序排序）
  qsort(tasks, taskCount, sizeof(TaskInfo), compare);

  // 打印排序后的任务
  printf("Tasks sorted by execution time:\n");
  for (int i = 0; i < taskCount; i++)
  {
    printf("Task: %s, Execution Time: %d\n", tasks[i].taskName, tasks[i].executionTime);
  }

  // 在这里，我们模拟执行时间最短的任务先执行
  for (int i = 0; i < taskCount; i++)
  {
    // 启动任务（模拟贪心策略）
    printf("Starting Task %s with execution time: %d\n", tasks[i].taskName, tasks[i].executionTime);
    // 任务执行
    vTaskResume(tasks[i].taskHandle); // 恢复任务
  }
}


void setupTask(void *pvParameters) {
    // 创建任务
    TaskHandle_t taskAHandle, taskBHandle, taskCHandle;

    xTaskCreate(TaskA, "TaskA", 128, NULL, osPriorityNormal, &taskAHandle);
    xTaskCreate(TaskB, "TaskB", 128, NULL, osPriorityNormal, &taskBHandle);
    xTaskCreate(TaskC, "TaskC", 128, NULL, osPriorityNormal, &taskCHandle);

    // 创建任务信息数组
    TaskInfo tasks[] = {
        {taskAHandle, 1000, "Task A"},
        {taskBHandle, 2000, "Task B"},
        {taskCHandle, 500, "Task C"}
    };

    int taskCount = sizeof(tasks) / sizeof(TaskInfo);

    // 启动任务（初始状态为挂起）
    for (int i = 0; i < taskCount; i++) {
        vTaskSuspend(tasks[i].taskHandle); // 挂起所有任务
    }

    // 自定义调度器模拟贪心调度（选择执行时间最短的任务）
    customScheduler(tasks, taskCount);

    vTaskDelete(NULL);
    
}




/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void User_Tasks_Init(void)
{
    xTaskCreate(setupTask, "Setup Task", 128, NULL, osPriorityNormal, NULL);
}

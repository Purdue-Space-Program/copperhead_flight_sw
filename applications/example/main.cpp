#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"

#include <stdbool.h>
#include <stdio.h>

#include <chrono>

void PrintTask(void *argument);

#define TASK_STACK_SIZE 1024
#define TASK_STATIC_SIZE 1024

int main(void)
{
    StackType_t task_stack[TASK_STACK_SIZE] = {0};
    StaticTask_t idk_bruh[TASK_STATIC_SIZE] = {{0}};
    xTaskCreateStatic(PrintTask, "Print", TASK_STACK_SIZE, NULL, 1, task_stack, idk_bruh);

    vTaskStartScheduler();

    while (true)
    {
    }
}

/* PrintTask: prints a message every 1000 ms */
void PrintTask(void *argument)
{
    (void)argument;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;)
    {
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000));

        auto time = static_cast<long long>(std::chrono::system_clock::now().time_since_epoch().count());
        printf("[%lld] Hello world!\n", time);
    }
}

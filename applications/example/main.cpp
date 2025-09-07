#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"

#include <stdbool.h>
#include <stdio.h>

#include <chrono>

void PrintTask(void *argument);

int main(void)
{
    StackType_t task_stack[1024] = {0};
    StaticTask_t idk_bruh[1024] = {0};
    xTaskCreateStatic(PrintTask, "Print", 256, NULL, 1, task_stack, idk_bruh);

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

        auto time = std::chrono::system_clock::now().time_since_epoch().count();
        printf("[%ld] Hello world!\n", time);
    }
}

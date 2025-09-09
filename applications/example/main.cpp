#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"

#include <stdbool.h>
#include <stdio.h>

#include <chrono>

#include "stm32h7xx_hal.h"

void PrintTask(void *argument);
void BlinkTask(void *argument);

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    StackType_t task_stack[1024] = {0};
    StaticTask_t idk_bruh[1024] = {0};
    /*xTaskCreateStatic(PrintTask, "Print", 256, NULL, 1, task_stack, idk_bruh);*/
    xTaskCreateStatic(BlinkTask, "Blink", 128, NULL, 1, NULL);
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

void BlinkTask(void *argument) {
    (void)argument;

    TickType_t lastWakeTime = xTaskGetTickCount();

    for (;;)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_3);
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(1000));

    }
}

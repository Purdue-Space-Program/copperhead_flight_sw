#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"

#include <stdbool.h>
#include <stdio.h>

#include <chrono>

extern "C" {
#include "stm32h7xx_hal.h"
}

void PrintTask(void *argument);
void BlinkTask(void *argument);

int main(void)
{
    HAL_Init();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
/*
    StackType_t task_stack[1024] = {0};
    StaticTask_t idk_bruh[1024] = {0};
 
    xTaskCreateStatic(PrintTask, "Print", 256, NULL, 1, task_stack, idk_bruh);
    xTaskCreateStatic(BlinkTask, "Blink", 128, NULL, 1, task_stack, idk_bruh);
    vTaskStartScheduler();
*/
    while (true)
    {
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_1);
        HAL_Delay(500);
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

    while (1)
    {
       
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_1);
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(1000));
        
    }
}

#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"

#include <stdbool.h>
#include <stdio.h>

#include <chrono>




void PrintTask(void *argument);
void BlinkTask(void *argument);

#define TASK_STACK_SIZE 1024
#define TASK_STATIC_SIZE 1024

#define RCC_AHB4ENR (0x58024400 + 0x0E0)
#define GPIOB_BASE 0x58020400

void simple_delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}

int main(void)
{
    /* HAL_Init();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);*/
    
    *(volatile uint32_t*)(RCC_AHB4ENR) |= (1 << 1);
    
    *(volatile uint32_t*)(GPIOB_BASE) |= (1 << 0);
    *(volatile uint32_t*)(GPIOB_BASE) &= ~(1 << 1);

    *(volatile uint32_t*)(GPIOB_BASE + 0x04) &= ~(1 << 0);


    while (1) {

        *(volatile uint32_t*)(GPIOB_BASE + 0x18) = (1 << 0);
        simple_delay(2000000);
        *(volatile uint32_t*)(GPIOB_BASE + 0x18) = (1 << 16);
        simple_delay(2000000);

    }
    /*
    StackType_t task_stack[1024] = {0};
    StaticTask_t idk_bruh[1024] = {0};
    xTaskCreateStatic(PrintTask, "Print", 256, NULL, 1, task_stack, idk_bruh);
    xTaskCreate(BlinkTask, "Blink", 256, NULL, task_stack, idk_bruh);
    StackType_t task_stack[TASK_STACK_SIZE] = {0};
    StaticTask_t idk_bruh[TASK_STATIC_SIZE] = {{0}};
    xTaskCreateStatic(PrintTask, "Print", TASK_STACK_SIZE, NULL, 1, task_stack, idk_bruh);

    vTaskStartScheduler();
    */
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

void BlinkTask(void *argument)
{
    (void)argument;

    TickType_t lastWakeTime = xTaskGetTickCount();

    for (;;)
    {
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(500));
    }
}

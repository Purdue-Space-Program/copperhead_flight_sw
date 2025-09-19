#include "stm32h7xx_hal.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"


#define DELAY 2000000

void blink_task(void* pvParameters) {
    
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    while (true) {
        // Toggle LED
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        vTaskDelay(pdMS_TO_TICKS(DELAY));
    }
}


int main() {

    HAL_Init();

    xTaskCreate(blink_task, "Blink Task", 128, NULL, 1, NULL);
    vTaskStartScheduler();
    while (true) {
    
    }

    return 0;
}

void assert_failed(uint8_t* file, uint32_t line) {
    printf("Assertion failed in file %s on line %lu\n", file, line);
    while (true) {
        // Stay here
    }
}
#include "stm32h7xx_hal.h"
#include <stdint.h>
#include <stdio.h>
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

void vBlinkTask1(void *pvParameters);
int main()
{
    HAL_Init();
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    xTaskCreate(
                vBlinkTask1,
                "Blink",
                256,
                NULL,
                tskIDLE_PRIORITY+1,
                NULL
            );

    vTaskStartScheduler();

    //Should never reach here
    for (;;) {

    }
}

void vBlinkTask1(void *pvParameters) {
    (void) pvParameters;

    for (;;) {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add their own implementation to report the file name and line,
       e.g., printf("Assertion failed: %s:%lu\n", file, line) */
    (void)file;
    (void)line;
    /* Loop forever to aid debugging */
    while (true)
    {
        __asm__("bkpt #0");
    }
}
#endif

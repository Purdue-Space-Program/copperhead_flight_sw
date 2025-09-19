#include <stdio.h>
#include "task.h"
#include <stdint.h>

//Temp for testing flash
#define RCC_AHB4ENR (0x58024400 + 0x0E0)
#define GPIOB_BASE 0x58020400
#define DELAY 1000000
#define BSB0 0
#define BRB0 16
#define BSBR 0x18
#define DELAY 2000000

void simple_delay(uint32_t count) {
    while (count--) {
        asm("nop");
    }
}

int main() {
    
    //Clock
    *(volatile uint32_t *)(RCC_AHB4ENR) |= (1 << 1);
    //Set as output
    *(volatile uint32_t *)(GPIOB_BASE) |= (1 << 0);
    *(volatile uint32_t *)(GPIOB_BASE) &= ~(1 << 1);
    //Push pull output
    *(volatile uint32_t *)(GPIOB_BASE + 0x04) &= ~(1 << 0);

    while (true) {
        *(volatile uint32_t *)(GPIOB_BASE + BSBR) = (1 << BSB0);
        simple_delay(DELAY);
        *(volatile uint32_t *)(GPIOB_BASE + BSBR) = (1 << BRB0);
        simple_delay(DELAY);
    }

    return 0;
}


void assert_failed(uint8_t* file, uint32_t line) {
    printf("Assertion failed in file %s on line %lu\n", file, line);
    while (true) {
        // Stay here
    }
}
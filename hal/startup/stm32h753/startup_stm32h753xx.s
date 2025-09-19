.syntax unified
.cpu cortex-m7
.fpu fpv5-d16
.thumb

/* Make Reset_Handler globally visible */
.global Reset_Handler
.global Default_Handler

/* Stack pointer initial value (end of DTCMRAM) */
.word _estack

/* Vector table */
.section .isr_vector,"a",%progbits
.type g_pfnVectors, %object
.size g_pfnVectors, .-g_pfnVectors

g_pfnVectors:
    .word _estack                    /* 0x000 Stack pointer */
    .word Reset_Handler              /* 0x004 Reset */
    .word NMI_Handler                /* 0x008 NMI */
    .word HardFault_Handler          /* 0x00C Hard fault */
    .word MemManage_Handler          /* 0x010 Memory management */
    .word BusFault_Handler           /* 0x014 Bus fault */
    .word UsageFault_Handler         /* 0x018 Usage fault */
    .word 0                          /* 0x01C Reserved */
    .word 0                          /* 0x020 Reserved */
    .word 0                          /* 0x024 Reserved */
    .word 0                          /* 0x028 Reserved */
    .word SVC_Handler                /* 0x02C SVCall */
    .word DebugMon_Handler           /* 0x030 Debug monitor */
    .word 0                          /* 0x034 Reserved */
    .word PendSV_Handler             /* 0x038 PendSV */
    .word SysTick_Handler            /* 0x03C SysTick */

    /* External interrupts */
    .word Default_Handler            /* WWDG */
    .word Default_Handler            /* PVD_PVM */
    .word Default_Handler            /* RTC_TAMP_STAMP */
    .word Default_Handler            /* RTC_WKUP */
    .word Default_Handler            /* FLASH */
    .word Default_Handler            /* RCC */
    .word Default_Handler            /* EXTI0 */
    .word Default_Handler            /* EXTI1 */
    .word Default_Handler            /* EXTI2 */
    .word Default_Handler            /* EXTI3 */
    .word Default_Handler            /* EXTI4 */
    .word Default_Handler            /* DMA1_Stream0 */
    .word Default_Handler            /* DMA1_Stream1 */
    .word Default_Handler            /* DMA1_Stream2 */
    .word Default_Handler            /* DMA1_Stream3 */
    .word Default_Handler            /* DMA1_Stream4 */
    .word Default_Handler            /* DMA1_Stream5 */
    .word Default_Handler            /* DMA1_Stream6 */
    .word Default_Handler            /* ADC */

.text

/*
 * Reset handler - entry point after reset
 */
.section .text.Reset_Handler
.weak Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
    /* Copy initialized data from flash to RAM */
    ldr r0, =_sdata      /* Destination start */
    ldr r1, =_edata      /* Destination end */
    ldr r2, =_sidata     /* Source start */
    movs r3, #0
    b copy_data_check

copy_data_loop:
    ldr r4, [r2, r3]     /* Load from flash */
    str r4, [r0, r3]     /* Store to RAM */
    adds r3, r3, #4

copy_data_check:
    adds r4, r0, r3
    cmp r4, r1
    bcc copy_data_loop

    /* Zero out BSS section */
    ldr r2, =_sbss
    ldr r4, =_ebss
    movs r3, #0
    b zero_bss_check

zero_bss_loop:
    str r3, [r2]
    adds r2, r2, #4

zero_bss_check:
    cmp r2, r4
    bcc zero_bss_loop

    /* Enable FPU */
    ldr r0, =0xE000ED88  /* CPACR */
    ldr r1, [r0]
    orr r1, r1, #0xF00000  /* Enable CP10 and CP11 */
    str r1, [r0]
    dsb
    isb

    /* Call main function */
    bl main
    
    /* Infinite loop if main returns */
hang:
    b hang

.size Reset_Handler, .-Reset_Handler

/*
 * Default handler for unhandled interrupts
 */
.section .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
    b Infinite_Loop
.size Default_Handler, .-Default_Handler

/*
 * Weak definitions for exception handlers
 */
.weak NMI_Handler
.thumb_set NMI_Handler,Default_Handler

.weak HardFault_Handler
.thumb_set HardFault_Handler,Default_Handler

.weak MemManage_Handler
.thumb_set MemManage_Handler,Default_Handler

.weak BusFault_Handler
.thumb_set BusFault_Handler,Default_Handler

.weak UsageFault_Handler
.thumb_set UsageFault_Handler,Default_Handler

.weak SVC_Handler
.thumb_set SVC_Handler,Default_Handler

.weak DebugMon_Handler
.thumb_set DebugMon_Handler,Default_Handler

.weak PendSV_Handler
.thumb_set PendSV_Handler,Default_Handler

.weak SysTick_Handler
.thumb_set SysTick_Handler,Default_Handler

.end

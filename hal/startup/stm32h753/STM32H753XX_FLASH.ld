/* STM32H753ZI Memory Layout */
MEMORY
{
    FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 2048K
    DTCMRAM (xrw)   : ORIGIN = 0x20000000, LENGTH = 128K
    RAM_D1 (xrw)    : ORIGIN = 0x24000000, LENGTH = 512K
    RAM_D2 (xrw)    : ORIGIN = 0x30000000, LENGTH = 288K
    RAM_D3 (xrw)    : ORIGIN = 0x38000000, LENGTH = 64K
    ITCMRAM (xrw)   : ORIGIN = 0x00000000, LENGTH = 64K
}

/* Stack size */
_stack_size = 0x2000; /* 8KB stack */

/* Entry point */
ENTRY(Reset_Handler)

SECTIONS
{
    /* Vector table goes first in flash */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } >FLASH

    /* Program code and constants */
    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        *(.glue_7)
        *(.glue_7t)
        *(.eh_frame)

        KEEP (*(.init))
        KEEP (*(.fini))

        . = ALIGN(4);
        _etext = .;
    } >FLASH

    /* Read-only data */
    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } >FLASH

    /* ARM specific sections */
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } >FLASH
    
    .ARM :
    {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    } >FLASH

    /* Initialized data - will be copied from flash to RAM */
    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } >DTCMRAM AT> FLASH
    
    _sidata = LOADADDR(.data);

    /* Uninitialized data - will be zeroed */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = _ebss;
    } >DTCMRAM

    . = ALIGN(4);
    _end = .;
    end = _end;
    __end__ = _end;

    /* Stack */
    .stack :
    {
        . = ALIGN(8);
        . = . + _stack_size;
        . = ALIGN(8);
        _estack = .;
    } >DTCMRAM

    /* Remove debugging info */
    /DISCARD/ :
    {
        libc.a ( * )
        libm.a ( * )
        libgcc.a ( * )
    }
}

# Drivers

## CMSIS
Stands for "common microcontroller software interface standard". Contains very
very low level details about ARM cores. The microcontroller we are using uses
the ARM Cortex M7.

## HAL
Stands for "hardware abstraction layer". This directory contains code that will
interface with various peripherals from our microcontroller. We will be using
the manufacturer provided HAL until our in-house solution is feature-complete.
Will use CMSIS.

## IC
Stands for "integrated circuit". This directory contains code that interfaces
with hardware outside of our microcontroller. Will use the HAL.

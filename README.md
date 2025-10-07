# Copperhead Flight Software

All software that will run on the Copperhead vehicle.

## Requirements
- arm-none-eabi toolchain
- CMake
- Make or Ninja
  - Ninja is significantly faster than Make
- clang-format and clang-tidy for formatting and linting, respectively
- OpenOCD for flashing

## Building
Make sure submodules are pulled in:
```bash
git submodule init
git submodule update --depth=1
```
And then start the build:
```bash
cmake -B build -DTARGET=<TARGET>
cmake --build build
```

To flash, (only available for MCU) run: 
```bash
cmake -B buuld -DTARGET=<TARGET>
cmake --build build --target flash
```

Supported Targets:
1. `STM32H730`
2. `Native`
3. `STM32H753`

# Copperhead Flight Software

All software that will run on the Copperhead vehicle.

## Requirements
- arm-none-eabi toolchain
- CMake
- Make or Ninja
  - Ninja is significantly faster than Make

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

Supported Targets:
1. `STM32H730`
2. `Native`

#!/usr/bin/bash

rm -rf build/
cmake -B build -G Ninja -DTARGET=STM32H730
cmake --build build

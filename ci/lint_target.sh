#!/usr/bin/bash

rm -rf build/

cmake -B build -G Ninja -DTARGET=$1
run-clang-tidy \
    -p build \
    -extra-arg=-isystem/usr/lib/arm-none-eabi/include \
    -extra-arg=-isystem/usr/lib/gcc/arm-none-eabi/10.3.1/include \
    -extra-arg=-isystem/usr/lib/gcc/arm-none-eabi/10.3.1/include-fixed \
    -extra-arg=-isystem/usr/lib/arm-none-eabi/include/c++/10.3.1 \
    -extra-arg=-isystem/usr/lib/arm-none-eabi/include/c++/10.3.1/arm-none-eabi \
    -extra-arg=-isystem/usr/lib/arm-none-eabi/include/c++/10.3.1/backward \
    applications/ drivers/

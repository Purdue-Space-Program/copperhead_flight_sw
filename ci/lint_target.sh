#!/usr/bin/bash

rm -rf build/

cmake -B build -G Ninja -DTARGET=$1

GCC_VER=$(arm-none-eabi-gcc -dumpversion)

run-clang-tidy \
  -p build \
  -extra-arg=-isystem/usr/lib/arm-none-eabi/include \
  -extra-arg=-isystem/usr/lib/gcc/arm-none-eabi/$GCC_VER/include \
  -extra-arg=-isystem/usr/lib/gcc/arm-none-eabi/$GCC_VER/include-fixed \
  -extra-arg=-isystem/usr/lib/arm-none-eabi/include/c++/$GCC_VER \
  -extra-arg=-isystem/usr/lib/arm-none-eabi/include/c++/$GCC_VER/arm-none-eabi \
  -extra-arg=-isystem/usr/lib/arm-none-eabi/include/c++/$GCC_VER/backward \
  applications/ drivers/

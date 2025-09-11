#!/usr/bin/bash

rm -rf build/

cmake -B build -G Ninja -DTARGET=$1
run-clang-tidy -p build applications/ drivers/

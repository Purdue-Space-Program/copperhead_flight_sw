#!/usr/bin/bash

rm -rf build/
cmake -B build -G Ninja -DTARGET=Native -DCMAKE_BUILD_TYPE=Release
cmake --build build

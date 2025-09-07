#!/usr/bin/bash

files=$(git ls-files '*.cpp' '*.h' '*.c')
clang-format -i $files -Werror

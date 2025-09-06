#!/usr/bin/bash

files=$(git ls-files '*.cpp' '*.h' '*.c')
clang-format -i $files -Werror -n > /dev/null 2>&1
output=$?

if [[ "$output" -ne 0 ]]; then
    echo -e "\033[1;31mStyle error detected! Make sure to run clang-format!\033[0m"
    exit 1
else
    echo -e "\033[0;32mGood format detected!\033[0m"
fi

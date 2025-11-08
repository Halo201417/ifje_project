#!/bin/bash
echo "Compiling..."
gcc -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -o expr ./src/*.c || { echo "Compilation failed"; exit 1; }

echo "Running test..."
./expr < tests/test1.in > out1.txt || { echo "Execution failed"; exit 1; }

echo "Comparing results..."
if diff -u tests/test1.out out1.txt; then
    echo "Test passed"
else
    echo "Test failed"
fi

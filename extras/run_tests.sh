#!/bin/bash
echo "Compiling..."
gcc -std=c11 -Wall -Wextra -g src/*.c -o expr || { echo "Compilation failed"; exit 1; }

echo "Running test..."
./expr < tests/test1.in > out1.txt || { echo "Execution failed"; exit 1; }

echo "Comparing results..."
if diff -u tests/test1.out out1.txt; then
    echo "Test passed"
else
    echo "Test failed"
fi

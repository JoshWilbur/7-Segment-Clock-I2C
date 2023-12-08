#!/bin/bash

# Move to directory
cd 7-Segment-Clock-I2C

# Compile clock program
if gcc 7Seg.c numbers.c -o clock; then
    # Run program
    sudo ./clock
else
    echo "Compilation failed. Check for errors."
fi

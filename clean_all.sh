#!/bin/bash

# Script to clean all subdirectories containing a Makefile

# Find all subdirectories with a Makefile
for dir in $(find . -type f -name "Makefile" -exec dirname {} \;); do
    echo "Cleaning in $dir..."
    (cd "$dir" && make clean)
done

echo "Clean-up complete!"


#!/bin/bash

# Script to compile all subdirectories containing a Makefile

# Find all subdirectories with a Makefile
for dir in $(find . -type f -name "Makefile" -exec dirname {} \;); do
    echo "
    Compiling in $dir..."
    (cd "$dir" && make)
done

echo "Compilation complete!"

#!/bin/bash

# Create the build directory
mkdir -p build

# Change into the build directory
cd ./build

# Run CMake to generate the solution
cmake .. -DCMAKE_BUILD_TYPE=Debug
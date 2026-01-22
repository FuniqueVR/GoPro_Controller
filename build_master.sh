#!/bin/bash

# Create build directory
mkdir -p build
cd build

# Configure CMake
cmake . -DBUILD_SERVER=OFF -DBUILD_MASTER=ON

cmake --build .

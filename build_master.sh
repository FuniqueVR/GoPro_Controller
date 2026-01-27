#!/bin/bash

# Create build directory
mkdir -p build_master
cd build_master

# Configure CMake
cmake .. -DBUILD_SERVER=OFF -DBUILD_MASTER=ON

cmake --build . -j 8

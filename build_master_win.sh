#!/bin/bash

# Create build directory
mkdir -p build_master_win
cd build_master_win

# Configure CMake
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../cmake/windows-toolchain.cmake -DBUILD_SERVER=OFF -DBUILD_MASTER=ON

cmake --build . -j 8

#!/bin/bash

# Clean previous build artifacts seriously to avoid cache issues
rm -rf CMakeCache.txt CMakeFiles build_server

# Create a separate build directory for server
mkdir -p build_server
cd build_server

# Configure CMake with Toolchain
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../cmake/arm-gnueabihf.toolchain.cmake -DBUILD_SERVER=ON -DBUILD_MASTER=OFF

# Build
cmake --build . -j 8

#!/bin/bash

# Create a separate build directory for server
mkdir -p build_server_win
cd build_server_win

# Configure CMake with Toolchain
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../cmake/windows-toolchain.cmake -DBUILD_SERVER=ON -DBUILD_MASTER=OFF

# Build
cmake --build . -j 8

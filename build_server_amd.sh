#!/bin/bash

# Clean previous build artifacts seriously to avoid cache issues
rm -rf CMakeCache.txt CMakeFiles build_server_amd

# Create a separate build directory for server
mkdir -p build_server_amd
cd build_server_amd

# Configure CMake with Toolchain
cmake .. -DCMAKE_BUILD_TYPE=Debug -DBUILD_SERVER=ON -DBUILD_MASTER=OFF

# Build
cmake --build . -j 8

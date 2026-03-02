#!/bin/bash

# Create build directory
mkdir -p build_master_win
cd build_master_win

cp ../Roboto-Medium.ttf .

# Configure CMake
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../cmake/msvc.toolchain.cmake -DCMAKE_CXX_FLAGS="-Wa,-mbig-obj" -DBUILD_SERVER=OFF -DBUILD_MASTER=ON

# Build
cmake --build . -j $nproc
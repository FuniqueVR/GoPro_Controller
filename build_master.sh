#!/bin/bash

# Create build directory
mkdir -p build_master
cd build_master

cp ../*.otf .

# Configure CMake
cmake .. -DCMAKE_BUILD_TYPE=Debug -DBUILD_SERVER=OFF -DBUILD_MASTER=ON

# Build
cmake --build . -j $nproc
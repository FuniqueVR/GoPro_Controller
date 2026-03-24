#!/bin/bash

# Create build directory
mkdir -p package_server_amd
cd package_server_amd

cp ../build_server_amd/server server_linux_amd64
cp ../LICENSE LICENSE
cp ../README.md README.md

tar -czvf server_linux_amd64.tar.gz .
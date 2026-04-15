#!/bin/bash

# Create build directory
mkdir -p package_server
cd package_server

mkdir -p go_pro_control_server
cd go_pro_control_server

cp ../../build_server_amd/server server_linux_amd64
cp ../../LICENSE LICENSE
cp ../../README.md README.md

tar -czvf server_linux_amd64.tar.gz .

mkdir -p DEBIAN

cp -r ../../package/deb_server_amd64 ./DEBIAN
mv ./DEBIAN/deb_server_amd64 ./DEBIAN/control

cd ..

dpkg-deb --build go_pro_control_server
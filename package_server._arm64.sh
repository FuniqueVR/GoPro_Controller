#!/bin/bash

# Create build directory
mkdir -p package_server_arm64
cd package_server_arm64

mkdir -p go_pro_control_server
cd go_pro_control_server

mkdir -p bin

cp ../../build_server_arm64/server bin/go-pro-server

tar -czvf ../server_linux_arm64.tar.gz .

mkdir -p DEBIAN

cp -r ../../package/deb_server_arm64 ./DEBIAN
mv ./DEBIAN/deb_server_arm64 ./DEBIAN/control

cd ..

dpkg-deb --build go_pro_control_server
#!/bin/bash

# Create build directory
mkdir -p package_server_amd64
cd package_server_amd64

mkdir -p go_pro_control_server
cd go_pro_control_server

mkdir -p bin

cp ../../build_server_amd64/server bin/go-pro-server

tar -czvf ../server_linux_amd64.tar.gz .

mkdir -p DEBIAN

cp -r ../../package/deb_server_amd64 ./DEBIAN
mv ./DEBIAN/deb_server_amd64 ./DEBIAN/control

cd ..

dpkg-deb --build go_pro_control_server
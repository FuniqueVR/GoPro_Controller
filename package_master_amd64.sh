#!/bin/bash

# Create build directory
mkdir -p package_master_amd64
cd package_master_amd64

mkdir -p go_pro_control_master
cd go_pro_control_master

cp ../build_master_amd64/master master_linux_amd64
cp ../build_master_amd64/SourceHanSans-Medium.otf SourceHanSans-Medium.otf
cp ../build_master_amd64/SourceHanSansK-Medium.otf SourceHanSansK-Medium.otf
cp ../build_master_amd64/SourceHanSansTC-Medium.otf SourceHanSansTC-Medium.otf
cp ../LICENSE LICENSE
cp ../README.md README.md

tar -czvf master_linux_amd64.tar.gz .

mkdir -p DEBIAN

cp -r ../../package/deb_master_amd64 ./DEBIAN
mv ./DEBIAN/deb_master_amd64 ./DEBIAN/control

cd ..

dpkg-deb --build go_pro_control_server
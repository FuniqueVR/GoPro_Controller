#!/bin/bash

# Create build directory
mkdir -p package_master_amd64
cd package_master_amd64

mkdir -p go_pro_control_master
cd go_pro_control_master

mkdir -p bin
mkdir -p usr/share/fonts/truetype

cp ../../build_master_amd64/master bin/go-pro-master
cp ../../build_master_amd64/SourceHanSans-Medium.otf usr/share/fonts/truetype/SourceHanSans-Medium.otf
cp ../../build_master_amd64/SourceHanSansK-Medium.otf usr/share/fonts/truetype/SourceHanSansK-Medium.otf
cp ../../build_master_amd64/SourceHanSansTC-Medium.otf usr/share/fonts/truetype/SourceHanSansTC-Medium.otf

tar -czvf ../master_linux_amd64.tar.gz .

mkdir -p DEBIAN

cp -r ../../package/deb_master_amd64 ./DEBIAN
mv ./DEBIAN/deb_master_amd64 ./DEBIAN/control

cd ..

dpkg-deb --build go_pro_control_master
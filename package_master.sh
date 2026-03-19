#!/bin/bash

# Create build directory
mkdir -p package_master
cd package_master

cp ../build_master/master master_linux_amd64
cp ../build_master/SourceHanSans-Medium.otf SourceHanSans-Medium.otf
cp ../build_master/SourceHanSansK-Medium.otf SourceHanSansK-Medium.otf
cp ../build_master/SourceHanSansTC-Medium.otf SourceHanSansTC-Medium.otf
cp ../LICENSE LICENSE
cp ../README.md README.md

tar -czvf master_linux_amd64.tar.gz .
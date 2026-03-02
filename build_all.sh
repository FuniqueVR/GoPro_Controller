#!/bin/bash

sh build_master_win.sh
sh build_master.sh
sh build_server_amd.sh
sh build_server_win.sh
sh build_server.sh

mkdir -p bin

cp ./build_master_win/master ./bin/master_win_amd64.exe
cp ./build_master/master ./bin/master_linux_amd64.exe
cp ./build_server_amd/server ./bin/server_linux_amd64.exe
cp ./build_server_win/server ./bin/server_win_amd64.exe
cp ./build_master/server ./bin/server_linux_arm64.exe
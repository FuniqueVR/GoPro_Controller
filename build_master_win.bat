mkdir build_master_win
cd build_master_win

wget https://github.com/opencv/opencv/releases/download/4.10.0/opencv-4.10.0-windows.exe
7z x opencv-4.10.0-windows.exe

xcopy ..\Roboto-Medium.ttf .

cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../cmake/msvc.toolchain.cmake -DBUILD_SERVER=OFF -DBUILD_MASTER=ON

cmake --build . -j 8
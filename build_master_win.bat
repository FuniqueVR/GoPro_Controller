mkdir build_master_win

cd build_master_win

xcopy ..\Roboto-Medium.ttf . /Y

cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../cmake/msvc.toolchain.cmake -DBUILD_SERVER=OFF -DBUILD_MASTER=ON

cmake --build . -j 8
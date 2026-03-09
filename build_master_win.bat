mkdir build_master_win

cd build_master_win

xcopy ..\Roboto-Medium.ttf . /Y

cmake .. -G "Visual Studio 18 2026" -A x64 -DCMAKE_SYSTEM_PROCESSOR=AMD64 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../cmake/msvc.toolchain.cmake -DBUILD_SERVER=OFF -DBUILD_MASTER=ON

cmake . -G "Visual Studio 18 2026" -A x64 --build -j 8
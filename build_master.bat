mkdir build_master

cd build_master

xcopy ..\*.otf . /Y

cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../cmake/msvc.toolchain.cmake -DBUILD_SERVER=OFF -DBUILD_MASTER=ON

cmake --build . -j 8
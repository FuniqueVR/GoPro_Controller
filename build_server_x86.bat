mkdir build_server_x86
cd build_server_x86

cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/msvc.toolchain.cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_SERVER=ON -DBUILD_MASTER=OFF

cmake --build .

mkdir -p build_server
cd build_server

cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/msvc.toolchain.cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_SERVER=ON -DBUILD_MASTER=OFF

cmake --build . -j 8

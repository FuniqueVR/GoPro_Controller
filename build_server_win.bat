mkdir build_server_win
cd build_server_win

cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../cmake/msvc.toolchain.cmake -DBUILD_SERVER=ON -DBUILD_MASTER=OFF

cmake . --build -j 8

mkdir build_server_win
cd build_server_win

cmake .. -G "Visual Studio 18 2026" -A x86_64 -DCMAKE_SYSTEM_PROCESSOR=AMD64 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../cmake/msvc.toolchain.cmake -DBUILD_SERVER=ON -DBUILD_MASTER=OFF

cmake . -G "Visual Studio 18 2026" -A x86_64 --build -j 8

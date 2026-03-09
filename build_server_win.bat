mkdir build_server_win
cd build_server_win

cmake .. -G "Visual Studio 18 2026" -A x64 -DOPENCV_SKIP_SYSTEM_PROCESSOR_DETECTION=1 -DCMAKE_GENERATOR_PLATFORM=x64 -DCMAKE_SYSTEM_PROCESSOR=AMD64 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../cmake/msvc.toolchain.cmake -DBUILD_SERVER=ON -DBUILD_MASTER=OFF

cmake . -G "Visual Studio 18 2026" -A x64 --build -j 8

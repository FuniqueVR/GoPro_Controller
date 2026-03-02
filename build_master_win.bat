mkdir build_master_win
cd build_master_win

xcopy ../Roboto-Medium.ttf .

cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../cmake/msvc.toolchain.cmake -DCMAKE_CXX_FLAGS="-Wa,-mbig-obj" -DBUILD_SERVER=OFF -DBUILD_MASTER=ON

cmake --build . -j $nproc
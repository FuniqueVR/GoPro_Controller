mkdir package_server_x64
cd package_server_x64

xcopy ..\build_server_x64\Debug\server.exe . /Y
ren server.exe go-pro-server.exe

Compil32 /cc ../package/win_server_x64.iss
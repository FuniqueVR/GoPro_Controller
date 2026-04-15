mkdir package_master_x64
cd package_master_x64

xcopy ..\build_master_x64\bin\Debug\master.exe . /Y
xcopy ..\build_master_x64\bin\Debug\*.otf . /Y
xcopy ..\build_master_x64\bin\Debug\*.dll . /Y
ren master.exe go-pro-master.exe

Compil32 /cc ../package/win_master_x64.iss
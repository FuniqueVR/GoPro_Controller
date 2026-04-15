mkdir package_master_x64
cd package_master_x64

xcopy ..\build_master_x64\Debug\master.exe . /Y
xcopy ..\SourceHanSans-Medium.otf . /Y
xcopy ..\SourceHanSansK-Medium.otf . /Y
xcopy ..\SourceHanSansTC-Medium.otf . /Y
ren master.exe go-pro-master.exe
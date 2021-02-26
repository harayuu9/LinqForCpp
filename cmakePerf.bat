if not exist build mkdir build
cd build

if not exist %CD%\cmake_download.zip (
    bitsadmin /transfer download https://github.com/Kitware/CMake/releases/download/v3.20.0-rc1/cmake-3.20.0-rc1-windows-x86_64.zip %CD%\cmake_download.zip
    powershell Expand-Archive cmake_download.zip
)

set PATH=%CD%\cmake_download\cmake-3.20.0-rc1-windows-x86_64\bin;%PATH%
where cmake || echo can't find cmake path && exit /b

set IS_PERFORMANCE_TEST=TRUE
set CPP_VERSION=17
cmake ../project || pause
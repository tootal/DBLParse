@echo off
cd %~dp0%..

set BUILD_TYPE=Release
set INSTALL_PREFIX=C:\Users\%username%\AppData\Local\Programs\DBLParse

rmdir /s/q build
cmake -DCMAKE_INSTALL_PREFIX=%INSTALL_PREFIX% -B build .
cmake --build build --config %BUILD_TYPE%
cmake --install build --config %BUILD_TYPE%

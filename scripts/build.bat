@echo off
cd %~dp0%..

call "scripts\setenv.bat"

echo Building...
cmake -B build
cmake --build build --config Release

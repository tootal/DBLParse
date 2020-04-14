@echo off
echo Setting up environment for Qt usage...
set PATH=C:\Qt\Qt5.12.7\5.12.7\msvc2017_64\bin;%PATH%
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

echo hello
pause

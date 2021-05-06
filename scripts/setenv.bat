@echo off
cd %~dp0%..
echo Setting up environment...

set QT_VERSION=5.15.2
set QT_KIT=msvc2019_64
set QT_PATH=C:\Qt\%QT_VERSION%\%QT_KIT%
set Qt5_DIR=%QT_PATH%\lib\cmake\Qt5
set PATH=%QT_PATH%\bin;%PATH%

set VS_VERSION=2019
set VS_TYPE=Enterprise
set VCINSTALLDIR=C:\Program Files (x86)\Microsoft Visual Studio\%VS_VERSION%\%VS_TYPE%\VC
@REM call "%VCINSTALLDIR%\Auxiliary\Build\vcvarsall.bat" x64

echo Removing cache...
rmdir /s/q build
rmdir /s/q deploy

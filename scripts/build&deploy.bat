@echo off
echo Setting up environment...
set QT_VERSION=5.14.2
set QT_KIT=msvc2017_64
set VS_VERSION=2019
set PATH=C:\Qt\%QT_VERSION%\%QT_KIT%\bin;%PATH%
set VCINSTALLDIR=C:\Program Files (x86)\Microsoft Visual Studio\%VS_VERSION%\Community\VC
call "%VCINSTALLDIR%\Auxiliary\Build\vcvarsall.bat" x64
cd ..

echo Removing cache...
rmdir /s/q deploy

echo Building...
qmake
nmake

echo Deploying...
mkdir deploy
cd deploy
mkdir DBLParse
cd ..
copy src\release\DBLParse.exe deploy\DBLParse
cd deploy\DBLParse 
windeployqt DBLParse.exe

echo Compressing...
cd ..
bc c -fmt:7z DBLParse.7z DBLParse

echo Clean...
cd ..
nmake distclean

echo Deploy successful!
start deploy

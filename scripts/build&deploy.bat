@echo off
echo Setting up environment...
set QT_VERSION=5.14.2
set PATH=C:\Qt\%QT_VERSION%\msvc2017_64\bin;%PATH%
set VCINSTALLDIR=C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
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
bc c DBLParse.zip DBLParse

echo Clean...
cd ..
nmake distclean

echo Deploy successful!
start deploy

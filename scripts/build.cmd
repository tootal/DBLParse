@echo off
setlocal enabledelayedexpansion

echo Setting up environment...

set ARCH=x64
set QT_VERSION=5.15.0
set QT_KIT=msvc2019_64
set PATH=C:\Qt\%QT_VERSION%\%QT_KIT%\bin;%PATH%

if "%1"=="ci" (
    set VS_VERSION=Enterprise
) else (
    set VS_VERSION=Community
)

set VS_RELEASE=2019
set VCINSTALLDIR=C:\Program Files (x86)\Microsoft Visual Studio\%VS_RELEASE%\%VS_VERSION%\VC
call "%VCINSTALLDIR%\Auxiliary\Build\vcvarsall.bat" %ARCH%

echo Building...
cd ..
qmake
nmake

echo Deploying...
if exist deploy (
    rmdir /s/q deploy
)
mkdir deploy
cd deploy
mkdir DBLParse
cd ..
copy src\DBLParse.exe deploy\DBLParse
cd deploy\DBLParse 
windeployqt DBLParse.exe
copy "C:\Program Files\OpenSSL-Win64\bin\libcrypto-1_1-x64.dll" .
copy "C:\Program Files\OpenSSL-Win64\bin\libssl-1_1-x64.dll" .

echo Compressing...
cd ..
bc c -fmt:zip DBLParse.zip DBLParse

echo Clean...
cd ..
nmake distclean


echo Deploy successful!
if "%1"=="" (
    start deploy
)

endlocal
echo All operations have been completed!
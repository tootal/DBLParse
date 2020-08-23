@echo off
setlocal enabledelayedexpansion

set ARCH=x64
set LIBMODE=shared
set QT_BIN=C:\Qt\5.15.0\msvc2019_64\bin
if not "%1"=="" (
    if not "%2"=="" (
        set ARCH=%1
        set LIBMODE=%2
        set QT_BIN=C:\Qt\5.15.0-!ARCH!-!LIBMODE!\bin
    )
)
set PATH=%QT_BIN%;%PATH%
set DEST=deploy\dblp-%ARCH%-%LIBMODE%.exe

if "%1"=="ci" (
    set VS_VERSION=Enterprise
) else (
    set VS_VERSION=Community
)
set VS_RELEASE=2019
set VCINSTALLDIR=C:\Program Files (x86)\Microsoft Visual Studio\%VS_RELEASE%\%VS_VERSION%\VC
if not exist "%VCINSTALLDIR%\Auxiliary\Build\vcvarsall.bat" (
    echo vcvarsall.bat not found!
    exit /b 1
)

echo Setting up environment...
call "%VCINSTALLDIR%\Auxiliary\Build\vcvarsall.bat" %ARCH%

echo Building...
qmake
nmake

echo Deploying...
if not exist deploy (
    mkdir deploy
)

move dblp.exe %DEST%

echo Clean...
nmake distclean

endlocal

echo All operations have been completed!
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
echo %QT_BIN%
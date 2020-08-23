:: Usage: qt-build <arch> <libmode>
:: eg: qt-build x64 static
@echo off
setlocal

set ARCH=x64
if not "%1"=="" (
    set ARCH=%1
)

set LIBMODE=static
if not "%2"=="" (
    set LIBMODE=%2
)

set QT_SRC_PATH=C:\Qt\5.15.0\Src
set SHADOW_PATH=..\build
set VS_RELEASE=2019
set VS_VERSION=Community
:: Make sure each option has a prefix space
set QT_CONF_CPPSTD= c++17
set QT_CONF_LICENSE= -opensource -mp -confirm-license
if "%LIBMODE%"=="static" (
    set QT_CONF_LIBMODE= -static -static-runtime
) else (
    set QT_CONF_LIBMODE= -%LIBMODE%
)
set QT_CONF_PREFIX= C:\Qt\5.15.0-%ARCH%-%LIBMODE%
set QT_CONF_PART= -nomake examples -nomake tests
set QT_CONF_MODULE= -no-gui -no-widgets -no-dbus
set QT_CONF_THIRD= -no-glib -no-icu -no-zlib
set QT_CONF_NET= -no-ssl -no-openssl
set QT_CONF_GUI= -no-opengl -no-freetype
set QT_CONF_IMG= -no-gif -no-ico -no-libpng -no-libjpeg
set QT_CONF_SQL= -no-sql-db2 -no-sql-ibase -no-sql-mysql^
 -no-sql-oci -no-sql-odbc -no-sql-psql -no-sql-sqlite2 -no-sql-sqlite -no-sql-tds -no-sqlite

cd %QT_SRC_PATH%
if exist %SHADOW_PATH% (
    echo Removing cache...
    rmdir /s/q %SHADOW_PATH%
)
mkdir %SHADOW_PATH%
cd %SHADOW_PATH%

echo Setting up environment...
set VCINSTALLDIR=C:\Program Files (x86)\Microsoft Visual Studio\%VS_RELEASE%\%VS_VERSION%\VC
call "%VCINSTALLDIR%\Auxiliary\Build\vcvarsall.bat" %ARCH%

:: Start build qt
:: For more information, please check %QT_SRC_PATH%\README
call %QT_SRC_PATH%\qtbase\configure -c++std%QT_CONF_CPPSTD% -prefix%QT_CONF_PREFIX%^
%QT_CONF_LICENSE%^
%QT_CONF_LIBMODE%^
%QT_CONF_PART%^
%QT_CONF_MODULE%^
%QT_CONF_THIRD%^
%QT_CONF_NET%^
%QT_CONF_GUI%^
%QT_CONF_IMG%^
%QT_CONF_SQL%
nmake

nmake install
endlocal
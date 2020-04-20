@echo off
echo Setting up environment...
set PATH=C:\Qt\Qt5.12.7\5.12.7\msvc2017_64\bin;%PATH%
set VCINSTALLDIR=C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cd ..

echo Removing cache...
rmdir /s/q deploy

echo Building...
qmake --version
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

echo Generating nCr...
python ../../scripts/generate_nCr.py 

echo Compressing...
cd ..
bc c DBLParse.zip DBLParse

echo Clean...
cd ..
rmdir /s/q "pivoter/debug"
rmdir /s/q "pivoter/release"
rmdir /s/q "src/debug"
rmdir /s/q "src/release"
del /s/q Makefile*
del .qmake.stash

echo Deploy successful!
start deploy

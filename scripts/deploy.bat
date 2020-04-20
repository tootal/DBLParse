@echo off
echo Setting up environment for Qt usage...
set PATH=C:\Qt\Qt5.12.7\5.12.7\msvc2017_64\bin;%PATH%
set VCINSTALLDIR=C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC
echo Changing release path...
cd ..\..
echo Removing cache...
rmdir /s/q deploy-DBLParse
echo Generating files...
mkdir deploy-DBLParse
cd deploy-DBLParse
mkdir DBLParse
cd ..
copy build-DBLParse-Desktop_Qt_5_12_7_MSVC2017_64bit-Release\src\release\DBLParse.exe deploy-DBLParse\DBLParse
cd deploy-DBLParse\DBLParse 
windeployqt DBLParse.exe
echo Generating nCr...
python ../../DBLParse/scripts/generate_nCr.py 
cd ..
echo Compressing...
bc c DBLParse.zip DBLParse
echo Deploy successful!
start .
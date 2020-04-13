@echo off
echo Setting up environment for Qt usage...
set PATH=C:\Qt\Qt5.12.7\5.12.7\msvc2017_64\bin;%PATH%
echo Changing release path...
cd /D ..\build-DBLParse-Desktop_Qt_5_12_7_MSVC2017_64bit-Release\release
echo Removing cache...
rmdir /s/q DBLParse 
echo Generating files...
mkdir DBLParse
copy DBLParse.exe DBLParse
cd DBLParse
windeployqt DBLParse.exe
copy ..\..\..\DBLParse\degeneracy_cliques.exe .
copy ..\..\..\DBLParse\nCr.txt .
cd ..
echo Compressing...
bc c DBLParse.zip DBLParse
echo Deploy successful!
start .
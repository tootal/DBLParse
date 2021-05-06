@echo off
cd %~dp0%..

call "scripts\build.bat"

echo Deploying...
mkdir deploy\DBLParse
copy build\src\Release\DBLParse.exe deploy\DBLParse
windeployqt deploy\DBLParse\DBLParse.exe --release --verbose 0

echo Compressing...
@REM bc c -fmt:7z DBLParse.7z DBLParse
7z a deploy\DBLParse.zip deploy\DBLParse

echo Deploy successful!
pause

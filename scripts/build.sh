#!/bin/bash
cd $(cd `dirname $0`/..; pwd)

sudo apt install -y libglew-dev libglfw3-dev libxcb-xinerama0

rm -rf build
rm -rf deploy

cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

mkdir -p deploy/DBLParse
cp build/src/DBLParse deploy/DBLParse/
cp scripts/default.desktop deploy/DBLParse/
cp src/resources/DBLParse.png deploy/DBLParse/default.png

if [ ! -f "./linuxdeployqt-continuous-x86_64.AppImage" ]; then
wget https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage
chmod u+x linuxdeployqt-continuous-x86_64.AppImage
fi
./linuxdeployqt-continuous-x86_64.AppImage ./deploy/DBLParse/DBLParse -appimage -unsupported-allow-new-glibc
mv DBLParse-*-x86_64.AppImage DBLParse.AppImage

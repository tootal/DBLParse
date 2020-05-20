#!/bin/bash
cd /home/tootal/QtProjects/DBLParse
qmake
make
rm -rf deploy
mkdir deploy
cp src/DBLParse deploy/
cp scripts/default.desktop deploy/
cd deploy
linuxdeployqt DBLParse -appimage
cp DBLParse*.AppImage ~/Desktop/
cd ..
make clean
make distclean
rm -rf deploy


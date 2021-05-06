#!/bin/zsh
cd $(cd `dirname $0`/..; pwd)

echo Setting up environment...
export QT_VERSION=5.15.2
export QT_KIT=clang_64
export QT_PATH=${HOME}/Qt/${QT_VERSION}/${QT_KIT}
export Qt5_DIR=${QT_PATH}/lib/cmake/Qt5
export PATH=${QT_PATH}/bin:${PATH}

echo Removing cache...
rm -rf build
rm -rf deploy

echo Building...
cmake -B build
cmake --build build

echo Deploying...
mkdir -p deploy
cp -R build/src/DBLParse.app deploy

macdeployqt deploy/DBLParse.app -dmg -verbose=1

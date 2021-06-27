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
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

echo Deploying...
mkdir -p deploy/DBLParse
cp -R build/src/DBLParse.app deploy/DBLParse/

macdeployqt deploy/DBLParse/DBLParse.app
ln -s /Applications ./deploy/DBLParse/Applications
hdiutil create -srcfolder ./deploy/DBLParse -format UDBZ ./deploy/DBLParse.dmg

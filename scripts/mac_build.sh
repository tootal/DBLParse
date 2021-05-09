#!/bin/zsh
cd $(cd `dirname $0`/..; pwd)

echo Setting up environment...
export QT_VERSION=5.15.2
export QT_KIT=clang_64
export QT_PATH=${HOME}/Qt/${QT_VERSION}/${QT_KIT}
export Qt5_DIR=${QT_PATH}/lib/cmake/Qt5
export PATH=${QT_PATH}/bin:${PATH}

echo Removing cache...
rm -rf build/release

echo Building...
cmake -B build/release -DCMAKE_BUILD_TYPE=Release
cmake --build build/release

echo Deploying...
mkdir -p build/DBLParse
cp -R build/release/src/DBLParse.app build/DBLParse/

macdeployqt build/DBLParse/DBLParse.app
ln -s /Applications ./build/DBLParse/Applications
hdiutil create -srcfolder ./build/DBLParse -format UDBZ ./build/DBLParse.dmg

## Use aqt install qt sdk

```
sudo apt install python3 python3-pip
sudo pip3 install aqtinstall
sudo  python3 -m aqt install --outputdir /opt/Qt 5.15.0 linux desktop -m qtwebengine
```

## Use qt build on Ubuntu

```
sudo apt-get install -y libglew-dev libglfw3-dev
PATH=/opt/Qt/5.15.0/gcc_64/bin/:$PATH
qmake
make
```

## Linux deploy qt

```
git clone https://github.com/tootal/linuxdeployqt
cd linuxdeployqt
qmake
make

wget https://nixos.org/releases/patchelf/patchelf-0.9/patchelf-0.9.tar.bz2
tar xf patchelf-0.9.tar.bz2
( cd patchelf-0.9/ && ./configure  && make && sudo make install )
sudo wget -c "https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage" -O /usr/local/bin/appimagetool
sudo chmod a+x /usr/local/bin/appimagetool
export LD_LIBRARY_PATH=/opt/Qt/5.15.0/gcc_64/lib/:$LD_LIBRARY_PATH
../linuxdeployqt/bin/linuxdeployqt DBLParse -appimage
```
<!-- QT_PLUGIN_PATH=/opt/Qt/5.15.0/gcc_64/plugins/ -->

sudo apt install libxcb-xinerama0
[for github actions: ] sudo apt install libxcb-icccm4
export LD_LIBRARY_PATH=/opt/Qt/5.15.0/gcc_64/lib/:$LD_LIBRARY_PATH
export ARCH=x86_64
../linuxdeployqt/bin/linuxdeployqt DBLParse -appimage
mv DBLParse*.AppImage DBLParse.AppImage

## Update translation
lupdate src.pro
lrelease translation/DBLParse_zh_CN.ts
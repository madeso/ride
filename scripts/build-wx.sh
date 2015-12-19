#!/bin/bash
echo $CC
which $CC
ls

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
DIR=$DIR + "/../"

$CC $DIR/scripts/hello-world.c

cd $DIR
mkdir wx3
cd wx3
wget https://github.com/wxWidgets/wxWidgets/archive/WX_3_0_2.tar.gz -O wx.tar.gz
tar -xzf wx.tar.gz
cd wxWidgets-WX_3_0_2
mkdir gtk-build
cd gtk-build
../configure --enable-webview --disable-compat28
make

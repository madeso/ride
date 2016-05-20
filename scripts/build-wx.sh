#!/bin/bash
echo $CC
which $CC
ls

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
DIR=$DIR/../

$CC $DIR/scripts/hello-world.c

cd $DIR
mkdir wx3
cd wx3
wget https://github.com/wxWidgets/wxWidgets/archive/v3.1.0.tar.gz -O wx.tar.gz
tar -xzf wx.tar.gz > wxtar || cat wxtar
cd wxWidgets-3.1.0
mkdir gtk-build
cd gtk-build
../configure --enable-webview --disable-compat28
make > wx_build_log || cat wx_build_log

#!/bin/bash
set -ev

echo $CXX

#install ride dependencies
sudo add-apt-repository -y ppa:chris-lea/protobuf
sudo apt-get update -qq
sudo apt-get install -qq --assume-yes libgtk2.0-dev libprotobuf-dev protobuf-compiler libwebkit-dev graphviz doxygen

# update cmake and compiler: copied from install part of https://github.com/skystrife/cpptoml/blob/toml-v0.4.0/.travis.yml
sudo apt-get install libc6-i386
wget http://www.cmake.org/files/v3.3/cmake-3.3.2-Linux-i386.sh
sudo sh cmake-3.3.2-Linux-i386.sh --prefix=/usr/local --exclude-subdir

# credit: https://github.com/beark/ftl/
# install g++ 4.8, if tests are run with g++
if [ "`echo $CXX`" == "g++" ]; then
	sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test;
	sudo apt-get update;
	sudo apt-get install -qq g++-4.8;
	sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 50
fi

# build wxWidgtets
mkdir wx3
cd wx3
wget https://github.com/wxWidgets/wxWidgets/archive/WX_3_0_2.tar.gz -O wx.tar.gz
tar -xzf wx.tar.gz
ls
cd wxWidgets-WX_3_0_2
mkdir gtk-build
cd gtk-build
../configure --help
../configure --enable-webview --disable-compat28
make
sudo make install
wx-config --version

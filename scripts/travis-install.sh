#!/bin/bash
set -ev

# what is the current compiler?
echo $CXX
echo $CC
# which $CC

#are commands even run?
ls

#from http://unix.stackexchange.com/questions/190571/sudo-in-non-interactive-script
## Detect the user who launched the script
usr=$(env | grep SUDO_USER | cut -d= -f 2)

## Exit if the script was not launched by root or through sudo
if [ -z $usr ] && [ $USER = "root" ]
then
    echo "The script needs to run as root" && exit 1
fi

#install ride dependencies
apt-get update -qq
apt-get install -qq --assume-yes libgtk2.0-dev libwebkit-dev

# update cmake and compiler: copied from install part of https://github.com/skystrife/cpptoml/blob/toml-v0.4.0/.travis.yml
apt-get install libc6-i386
sudo -u $usr wget --no-check-certificate http://www.cmake.org/files/v3.3/cmake-3.3.2-Linux-i386.sh
sh cmake-3.3.2-Linux-i386.sh --prefix=/usr/local --exclude-subdir

# credit: https://github.com/beark/ftl/
# install g++ 4.8, if tests are run with g++
if [ "`echo $CXX`" == "g++" ]; then
	add-apt-repository -y ppa:ubuntu-toolchain-r/test;
	sudo apt-get update -qq;
	apt-get install -qq g++-4.8;
	update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 50
    export CXX="g++-4.8" CC="gcc-4.8"
fi

#build and install protobuf
mkdir $TRAVIS_BUILD_DIR/build-proto/
cd $TRAVIS_BUILD_DIR/build-proto/
wget https://github.com/google/protobuf/releases/download/v2.6.1/protobuf-2.6.1.tar.gz -O proto.tar.gz
tar -xzf proto.tar.gz
mv protobuf-2.6.1 pb
cd pb/
pwd
ls
autoreconf -i
./configure || cat config.log
make
make check
sudo make install

# build wxWidgtets
sudo -i $TRAVIS_BUILD_DIR/scripts/build-wx.sh

cd $TRAVIS_BUILD_DIR/wx3/wxWidgets-3.1.0/gtk-build/
cat config.log
make install
sudo -u $usr wx-config --version


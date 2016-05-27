#!/bin/bash
set -ev

# update and display enviroment variables
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
export LD_RUN_PATH=$LD_RUN_PATH:/usr/local/lib
echo "$LD_LIBRARY_PATH"
echo "$LD_RUN_PATH"

ls

#from http://unix.stackexchange.com/questions/190571/sudo-in-non-interactive-script
## Detect the user who launched the script
usr=$(env | grep SUDO_USER | cut -d= -f 2)

if [ "$TRAVIS_OS_NAME" = "linux" ]; then
  sudo apt-get update -qq
  sudo apt-get install -qq --assume-yes libgtk2.0-dev libwebkit-dev

  # update cmake and compiler: copied from install part of https://github.com/skystrife/cpptoml/blob/toml-v0.4.0/.travis.yml
  sudo apt-get install libc6-i386
  wget --no-check-certificate http://www.cmake.org/files/v3.3/cmake-3.3.2-Linux-i386.sh
  sudo sh cmake-3.3.2-Linux-i386.sh --prefix=/usr/local --exclude-subdir

  # credit: https://github.com/beark/ftl/
  # install g++ 4.8, if tests are run with g++
  if [ "`echo $CXX`" == "g++" ]; then
    sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test;
    sudo apt-get update -qq;
    sudo apt-get install -qq g++-4.8;
    sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 50
    export CXX="g++-4.8" CC="gcc-4.8"
  fi
fi

#build and install protobuf
mkdir $TRAVIS_BUILD_DIR/build-proto/
cd $TRAVIS_BUILD_DIR/build-proto/
wget https://github.com/google/protobuf/releases/download/v2.6.1/protobuf-2.6.1.tar.gz -O proto.tar.gz
tar -xzf proto.tar.gz &> prototar || cat prototar
mv protobuf-2.6.1 pb
cd pb/
pwd
ls
autoreconf -i
./configure  --disable-shared &> config.log|| cat config.log
make &> proto_build_log || cat proto_build_log
make check &> proto_check || cat proto_check
sudo make install

# build wxWidgtets
cd $TRAVIS_BUILD_DIR
mkdir wx3
cd wx3
wget https://github.com/wxWidgets/wxWidgets/archive/v3.1.0.tar.gz -O wx.tar.gz
tar -xzf wx.tar.gz &> wxtar || cat wxtar
cd wxWidgets-3.1.0
mkdir gtk-build
cd gtk-build
../configure --enable-webview --disable-compat28
make
sudo make install
wx-config --version


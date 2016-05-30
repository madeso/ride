#!/bin/bash
set -ev

# update and display enviroment variables
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
export LD_RUN_PATH=$LD_RUN_PATH:/usr/local/lib
echo "$LD_LIBRARY_PATH"
echo "$LD_RUN_PATH"

mkdir $TRAVIS_BUILD_DIR/deps/

#if [ "$TRAVIS_OS_NAME" = "linux" ]; then
#  wget --no-check-certificate https://cmake.org/files/v3.5/cmake-3.5.2-Linux-i386.sh
#  sh cmake-3.5.2-Linux-i386.sh --prefix=/usr/local --exclude-subdir
#fi

if [ "$TRAVIS_OS_NAME" = "osx" ]; then
  # expose cmake command for osx https://cmake.org/pipermail/cmake/2014-September/058760.html
  mkdir $TRAVIS_BUILD_DIR/osx-deps/
  cd $TRAVIS_BUILD_DIR/osx-deps/
  wget --no-check-certificate https://cmake.org/files/v3.5/cmake-3.5.2-Darwin-x86_64.tar.gz
  tar -xzf cmake-3.5.2-Darwin-x86_64.tar.gz
  cd cmake-3.5.2-Darwin-x86_64
  ls
  export PATH=$PATH:/$TRAVIS_BUILD_DIR/osx-deps/cmake-3.5.2-Darwin-x86_64/CMake.app/Contents/bin
fi

# display the cmake version
cmake --version

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
./configure  --prefix=$TRAVIS_BUILD_DIR/deps/ --disable-shared &> config.log|| cat config.log
make &> proto_build_log || cat proto_build_log
make check &> proto_check || cat proto_check
make install

# build wxWidgtets
cd $TRAVIS_BUILD_DIR
mkdir wx3
cd wx3
wget https://github.com/wxWidgets/wxWidgets/archive/v3.1.0.tar.gz -O wx.tar.gz
tar -xzf wx.tar.gz &> wxtar || cat wxtar
cd wxWidgets-3.1.0
mkdir gtk-build
cd gtk-build
../configure --prefix=$TRAVIS_BUILD_DIR/deps/ --enable-webview --disable-compat28
make
make install
# wx-config --version

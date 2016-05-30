#!/bin/bash
set -ev

${CC} --version

${CXX} --version

cmake --version

# update and display enviroment variables
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
export LD_RUN_PATH=$LD_RUN_PATH:/usr/local/lib
echo "$LD_LIBRARY_PATH"
echo "$LD_RUN_PATH"

mkdir $TRAVIS_BUILD_DIR/deps/

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

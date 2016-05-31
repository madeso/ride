#!/bin/bash
set -ev

CMAKEOSXARG=""

if [ "$TRAVIS_OS_NAME" = "osx" ]; then
  CMAKEOSXARG="-Dcmake_osx_deployment_target=10.10"
fi

cmake -DCMAKE_PREFIX_PATH=$TRAVIS_BUILD_DIR/deps/ -DCMAKE_INSTALL_PREFIX=$TRAVIS_BUILD_DIR/install-dist/ -DwxWidgets_CONFIG_OPTIONS="--prefix=$TRAVIS_BUILD_DIR/deps/" $CMAKEOSXARG .

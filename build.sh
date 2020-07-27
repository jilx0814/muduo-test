#!/bin/sh

set -x

SOURCE_DIR=`pwd`
BUILD_DIR=${BUILD_DIR:-../build}
BUILD_TYPE=${BUILD_TYPE:-release}
INSTALL_DIR=${INSTALL_DIR:-../${BUILD_TYPE}-install-test}
CXX=${CXX:-g++}

ln -sf $BUILD_DIR/$BUILD_TYPE-test/compile_commands.json

mkdir -p $BUILD_DIR/$BUILD_TYPE-test \
	&& cd $BUILD_DIR/$BUILD_TYPE-test \
	&& cmake \
		-DCMAKE_BUILD_TYPE=$BUILD_TYPE \
		-DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=NO \
		$SOURCE_DIR \
	&& make $*
	


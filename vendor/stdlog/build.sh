#!/bin/bash

#脚本当前目录
SCRIPT_HOME=$(cd `dirname $0` && pwd)
echo "SCRIPT_HOME = $SCRIPT_HOME"

# 源码目录
SOURCE_HOME=/home/mini/git/github/spdlog
# 发行方式
compile="Release"

mkdir -p build 
cd build

# DCMAKE_INSTALL_PREFIX make install 安装路径

cmake -DCMAKE_SYSTEM_NAME=Linux \
      -DCMAKE_BUILD_TYPE=$compile \
      -DBUILD_SHARED_LIBS=OFF \
      -DCMAKE_INSTALL_PREFIX=$SCRIPT_HOME \
      $SOURCE_HOME
make -j4

make install
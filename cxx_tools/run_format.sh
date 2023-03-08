#!/usr/bin/bash

echo "Start Formating Code..."

SCRIPT_HOME=$(cd `dirname $0` && pwd)
# 设置项目根目录
PROJECT_HOME=$(cd $SCRIPT_HOME/.. && pwd)

echo "SCRIPT_HOME = $SCRIPT_HOME"
echo "PROJECT_HOME = $PROJECT_HOME"

# 将src目录下文件格式化
find $PROJECT_HOME/src \
  -path '*/build' -prune -o \
  '(' \
  -iname "*.h" -o \
  -iname "*.hpp" -o \
  -iname "*.inc" -o \
  -iname "*.c" -o \
  -iname "*.cc" -o \
  -iname "*.cpp" \
  ')' \
  -print \
  | xargs clang-format -i

echo "Format Code Finished."


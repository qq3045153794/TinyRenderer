#!/usr/bin/env bash

echo "Start Tidy Code..."

SCRIPT_HOME=$(cd `dirname $0` && pwd)
PROJECT_HOME=$(cd $SCRIPT_HOME/.. && pwd)

echo "SCRIPT_HOME = $SCRIPT_HOME"
echo "PROJECT_HOME = $PROJECT_HOME"

find $PROJECT_HOME/src/asset \
  -iname '*.c' -o \
  -iname '*.cc' -o \
  -iname '*.cpp' \
  | xargs clang-tidy \
  --format-style="file" \
  -p $PROJECT_HOME/build

echo "Tidy Code Finished."

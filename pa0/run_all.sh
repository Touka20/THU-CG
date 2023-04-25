#!/usr/bin/env bash

# If project not ready, generate cmake file.
if [[ ! -d build ]]; then
  echo "good"
else
  rm -rf build
fi
cmake -B build
cmake --build build

# Run all testcases.
# You can comment some lines to disable the run of specific examples.
mkdir -p output
build/PA0 testcases/canvas01_basic.txt output/canvas01.bmp
build/PA0 testcases/canvas02_emoji.txt output/canvas02.bmp

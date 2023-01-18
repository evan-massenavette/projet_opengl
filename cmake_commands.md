# How to build using Cmake on the command line

## Configure a release build

cmake -S . -B build/ -D CMAKE_BUILD_TYPE=Debug

## Build release binaries

cmake --build build/

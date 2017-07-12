#!/bin/bash

# clear the env
rm -rf bin
rm -rf lib
rm -rf build

mkdir -p build
cd build

cmake ..
make -j8

cd ..

rm -rf build

mv bin/aiolos.app bin/aiolos

echo "build done..."
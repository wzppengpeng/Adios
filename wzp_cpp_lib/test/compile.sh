#!/bin/bash

rm -rf test

echo "compiling..."

g++ -std=c++11 -o test -fpermissive -ftemplate-depth=1000 -O2 -pthread -I../ test.cpp

echo "compile ok"

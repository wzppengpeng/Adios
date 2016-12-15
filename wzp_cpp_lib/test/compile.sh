#!/bin/bash

rm -rf test

echo "compiling..."

g++ -std=c++11 -o test -fpermissive -I../ test.cpp

echo "compile ok"

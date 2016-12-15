#!/bin/bash

rm -rf task

echo "compiling..."

g++ -o task -std=c++11 -fpermissive -I../ test_task_job.cpp

echo "compile ok"

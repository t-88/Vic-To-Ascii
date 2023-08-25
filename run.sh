#!/bin/bash

set -eux

# g++ main.cpp -o main `pkg-config --cflags --libs opencv4`
g++ main.cpp -o main 
./main -img imgs/nyan_cat.png
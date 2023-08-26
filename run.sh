#!/bin/bash

set -e

function help {
    echo help:
    echo '     no args      : build with default option'
    echo '     -h           : print help'
    echo '     -opencv      : build with opencv features'
}
if [ $# -eq 0 ]; then
    echo building with default options on
    g++ main.cpp -o Victa 
elif [ $# -eq 1 ]; then
    if [ $1 = "-opencv" ]; then
        echo "building with opencv"
        g++ main.cpp -o Victa -DVID `pkg-config --cflags --libs opencv4`
    elif [ $1 = "-h" ]; then
        help
    else
        echo "unkown args given"
    fi
else
    echo "unkown args given"
    help
fi

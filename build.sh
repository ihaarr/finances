#!/bin/sh

mkdir .build
cd .build
cmake ..
cpu=$(sysctl -n hw.physicalcpu)
make -j${cpu}

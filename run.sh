#!/bin/sh

clear
./build.sh
if [ "$?" -eq "0" ]; then
	./.build/finances
fi

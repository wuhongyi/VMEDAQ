#!/bin/sh

num=`cat /proc/cpuinfo | grep processor | wc -l`

rm sources/Core/anacore_dict*
make clean
./autogen.sh --prefix=$PWD
make -j$num
make install

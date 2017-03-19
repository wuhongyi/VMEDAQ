#!/bin/sh

rm sources/Core/anacore_dict*
make clean
./autogen.sh --prefix=$PWD
make
make install

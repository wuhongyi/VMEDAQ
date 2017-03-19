#!/bin/bash

PREFIX=$TARTSYS/bin

. setup.sh
./segidlistc.pl $TARTSYS/include/segidlist.hh 
qmake AnaLoop.pro
make clean
make
qmake AnaScaler.pro
make clean
make

mkdir $PREFIX

cp AnaLoop AnaScaler $PREFIX
pyuic4 AnafileEditor.ui  > ui_AnafileEditor.py
cp AnafileEditor.py ui_AnafileEditor.py $PREFIX

cd $PREFIX
ln -s AnafileEditor.py AnafileEditor

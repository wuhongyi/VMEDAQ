#!/bin/bash

module load texlive/2019

rm -rf ../docs/doctrees/
rm -rf ../docs/zh/*

cp index.html ../docs/
touch ../docs/.nojekyll

make clean

make html
make latexpdf

cp -r build/html/* ../docs/zh/
cp build/latex/vmedaq.pdf ../README_zh.pdf

make clean

cp source/README.rst ../README.rst

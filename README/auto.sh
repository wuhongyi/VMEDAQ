#!/bin/bash

module load texlive/2019

rm -rf ../docs/*

touch ../docs/.nojekyll

make clean

make html
make latexpdf

cp -r build/html/* ../docs/
cp build/latex/vmedaq.pdf ../README.pdf

make clean

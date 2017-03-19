#! /bin/bash

cd bbcaenvme
cd cmdvme
make clean
make
cd ../babies
make clean
make
ln -sf caenvmebabies babies

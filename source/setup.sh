#!/bin/bash

if [ `whoami` = "root" ];then 
    echo "当前为root用户，能够执行此脚本！" 
else 
    echo "请在ROOT权限下执行此脚本！！！"
    exit 1
fi

num=`cat /proc/cpuinfo | grep processor | wc -l`

cd tar
tar -xzvf CAENVMELib-2.50.tgz
cd CAENVMELib-2.50/lib
sh install_x64
cd ../../

tar -xvzf CAENComm-1.2-build20140211.tgz
cd CAENComm-1.2/lib
sh install_x64
cd ../../

tar -xvzf CAENUpgrader-1.6.3-build20170511.tgz
cd CAENUpgrader-1.6.3
./configure
make -j$num
make install
cd ..


rm -rf CAENVMELib-2.50  CAENComm-1.2 CAENUpgrader-1.6.3 
echo "Install all software done !"



#!/bin/bash
# setup.sh --- 
# 
# Description: 
# Author: Hongyi Wu(吴鸿毅)
# Email: wuhongyi@qq.com 
# Created: 一 12月  3 17:22:32 2018 (+0800)
# Last-Updated: 日 12月 26 13:38:53 2021 (+0800)
#           By: Hongyi Wu(吴鸿毅)
#     Update #: 10
# URL: http://wuhongyi.cn 

if [ `whoami` = "root" ];then 
    echo "---P--K--U---------V--M--E----D--A--Q-------------"
else 
    echo "==ERROR==========================================="
    echo "请在 ROOT 权限下执行本脚本！！！"
    echo "=================================================="
    exit 1
fi

# ------------------------------------------------------------------------------

num=`cat /proc/cpuinfo | grep processor | wc -l`

tar -xzvf CAENVMELib-2.50.tgz
cd CAENVMELib-2.50/lib
sh install_x64
cd ../../

# tar -xzvf CAENVMELib-3.3.0-build20210806.tgz
# cd CAENVMELib-3.3.0/lib
# sh install_x64
# cd ../../

tar -xvzf CAENComm-1.5.0-build20210804.tgz
cd CAENComm-1.5.0/lib
sh install_x64
cd ../../

tar -xvzf CAENUpgrader-1.7.2-Build20210921.tgz
cd CAENUpgrader-1.7.2
./configure
make -j$num
make install
cd ..


rm -rf CAENVMELib-2.50  CAENComm-1.5.0 CAENUpgrader-1.7.2
echo "Install all software done !"

# 
# setup.sh ends here

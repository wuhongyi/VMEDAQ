#!/bin/bash
# setup.sh --- 
# 
# Description: 
# Author: Hongyi Wu(吴鸿毅)
# Email: wuhongyi@qq.com 
# Created: 一 12月  3 17:22:32 2018 (+0800)
# Last-Updated: 三 9月  1 22:04:49 2021 (+0800)
#           By: Hongyi Wu(吴鸿毅)
#     Update #: 6
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

tar -xzvf CAENVMELib-3.2.0-build20210409.tgz
cd CAENVMELib-3.2.0/lib
sh install_x64
cd ../../

tar -xvzf CAENComm-1.4.1-build20210305.tgz
cd CAENComm-1.4.1/lib
sh install_x64
cd ../../

tar -xvzf CAENUpgrader-1.7.1-build20210402.tgz
cd CAENUpgrader-1.7.1
./configure
make -j$num
make install
cd ..


rm -rf CAENVMELib-3.2.0  CAENComm-1.4.1 CAENUpgrader-1.7.1
echo "Install all software done !"

# 
# setup.sh ends here

#!/bin/bash
# autoinstallbabirl.sh --- 
# 
# Description: 
# Author: Hongyi Wu(吴鸿毅)
# Email: wuhongyi@qq.com 
# Created: 二 8月 15 20:37:39 2017 (+0800)
# Last-Updated: 一 12月  3 17:23:48 2018 (+0800)
#           By: Hongyi Wu(吴鸿毅)
#     Update #: 12
# URL: http://wuhongyi.cn 

if [ `whoami` = "root" ];then
    echo "==ERROR==========================================="
    echo "当前为 root 权限，不能执行本脚本！"
    echo "请在非 ROOT 权限下执行本脚本！！！"
    echo "=================================================="
    exit 1
else 
    echo "---P--K--U---------V--M--E----D--A--Q-------------"
fi

num=`cat /proc/cpuinfo | grep processor | wc -l`

######################################################################

rm -rf ~/babirl  #删除存在的软连接

cp babirl170125.tar.gz ~
cd ~
tar -zxvf babirl170125.tar.gz
ln -sf babirl170125 babirl

echo '#define BABIRLDIR "/home/'$USER'/babirl"'  >> ~/babirl/include/userdefine.h
echo '#define PIDDIR    "/home/'$USER'/babirl/run"' >> ~/babirl/include/userdefine.h

cd babirl

make -j$num

yonghumingtihuan='s/babirl/'$USER'/g'
sed -i  $yonghumingtihuan /home/$USER/babirl/rcd/babimo


######################################################################

grep 'PATH=$PATH:/home/'$USER'/babirl/bin/' ~/.bashrc >/dev/null
if [ $? -eq 0 ]; then
    echo 'PATH=$PATH:/home/'$USER'/babirl/bin/ already exists in ~/.bashrc'
else
    echo 'PATH=$PATH:/home/'$USER'/babirl/bin/' >> ~/.bashrc
fi

grep 'export TARTSYS=/home/'$USER'/VMEDAQ/anaroot' ~/.bashrc >/dev/null
if [ $? -eq 0 ]; then
    echo 'export TARTSYS=/home/'$USER'/VMEDAQ/anaroot already exists in ~/.bashrc'
else
    echo 'export TARTSYS=/home/'$USER'/VMEDAQ/anaroot' >> ~/.bashrc
fi

grep 'export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TARTSYS/lib:$TARTSYS/sources/Core' ~/.bashrc >/dev/null
if [ $? -eq 0 ]; then
    echo 'export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TARTSYS/lib:$TARTSYS/sources/Core already exists in ~/.bashrc'
else
    echo 'export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TARTSYS/lib:$TARTSYS/sources/Core' >> ~/.bashrc
fi


# 
# autoinstallbabirl.sh ends here

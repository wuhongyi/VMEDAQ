#!/bin/bash
# autoinstallbabirl.sh --- 
# 
# Description: 
# Author: Hongyi Wu(吴鸿毅)
# Email: wuhongyi@qq.com 
# Created: 二 8月 15 20:37:39 2017 (+0800)
# Last-Updated: 三 8月 16 15:50:50 2017 (+0800)
#           By: Hongyi Wu(吴鸿毅)
#     Update #: 5
# URL: http://wuhongyi.cn 

if [ `whoami` = "root" ];then 
    echo "当前为root用户，不能够执行此脚本！"
    echo "请在ROOT权限下执行此脚本！！！"
    exit 1
else 
    echo "当前为非root用户，能够执行此脚本！"
fi

num=`cat /proc/cpuinfo | grep processor | wc -l`

######################################################################

cp babirl* ~
cd ~
tar -zxvf babirl150401.tar.gz
ln -sf babirl150401 babirl

echo '#define BABIRLDIR "/home/'$USER'/babirl"'  >> ~/babirl/include/userdefine.h
echo '#define PIDDIR    "/home/'$USER'/babirl/run"' >> ~/babirl/include/userdefine.h

cd babirl

make -j$num

yonghumingtihuan='s/babirl/'$USER'/g'
sed -i  $yonghumingtihuan /home/$USER/babirl/rcd/babimo


######################################################################

echo 'PATH=$PATH:/home/'$USER'/babirl/bin/' >> ~/.bashrc
echo 'export TARTSYS=/home/'$USER'/VMEDAQ/anaroot' >> ~/.bashrc
echo 'export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TARTSYS/lib:$TARTSYS/sources/Core' >> ~/.bashrc

# 
# autoinstallbabirl.sh ends here

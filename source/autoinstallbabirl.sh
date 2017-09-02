#!/bin/bash
# autoinstallbabirl.sh --- 
# 
# Description: 
# Author: Hongyi Wu(吴鸿毅)
# Email: wuhongyi@qq.com 
# Created: 二 8月 15 20:37:39 2017 (+0800)
# Last-Updated: 六 9月  2 21:23:32 2017 (+0800)
#           By: Hongyi Wu(吴鸿毅)
#     Update #: 6
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

rm -rf ~/babirl  #删除存在的软连接

cp babirl150401.tar.gz ~
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

grep 'PATH=$PATH:/home/'$USER'/babirl/bin/' ~/.bashrc >/dev/null
if [ $? -eq 0 ]; then
    
else
    echo 'PATH=$PATH:/home/'$USER'/babirl/bin/' >> ~/.bashrc
fi

grep 'export TARTSYS=/home/'$USER'/VMEDAQ/anaroot' ~/.bashrc >/dev/null
if [ $? -eq 0 ]; then
    
else
    echo 'export TARTSYS=/home/'$USER'/VMEDAQ/anaroot' >> ~/.bashrc
fi

grep 'export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TARTSYS/lib:$TARTSYS/sources/Core' ~/.bashrc >/dev/null
if [ $? -eq 0 ]; then
    
else
    echo 'export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TARTSYS/lib:$TARTSYS/sources/Core' >> ~/.bashrc
fi


# 
# autoinstallbabirl.sh ends here

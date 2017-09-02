#!/bin/bash
# afterinstallbabirl.sh --- 
# 
# Description: 
# Author: Hongyi Wu(吴鸿毅)
# Email: wuhongyi@qq.com 
# Created: 二 8月 15 20:56:42 2017 (+0800)
# Last-Updated: 六 9月  2 21:48:53 2017 (+0800)
#           By: Hongyi Wu(吴鸿毅)
#     Update #: 7
# URL: http://wuhongyi.cn 

#以下需要用户修改
#usernamedir="/home/wuhongyi"  #这里wuhongyi为当前用户名

# ------------------------------------------------------------------------------

if [ $# -eq 1 ]; then
    if [ ! -d '/home/'$1'' ]; then
	echo "REEOR: the user name $1 do not exit."
	exit 1
    fi
else
    echo "USAGE:    sh $0  [user name]"
    echo "eg.   sh  $0  wuhongyi"
    exit 1
fi


if [ `whoami` = "root" ];then 
    echo "当前为root用户，能够执行此脚本！" 
else 
    echo "请在ROOT权限下执行此脚本！！！"
    exit 1
fi

# ------------------------------------------------------------------------------

rm -rf /etc/rc2.d/S99babimo /etc/rc3.d/S99babimo /etc/rc5.d/S99babimo

cp /home/$1/babirl/rcd/babimo /etc/init.d  
ln -sf /etc/init.d/babimo /etc/rc2.d/S99babimo
ln -sf /etc/init.d/babimo /etc/rc3.d/S99babimo
ln -sf /etc/init.d/babimo /etc/rc5.d/S99babimo


grep 'PATH=$PATH:'$usernamedir'/babirl/bin/' ~/.bashrc >/dev/null
if [ $? -eq 0 ]; then
    
else
    echo 'PATH=$PATH:'$usernamedir'/babirl/bin/' >> ~/.bashrc
fi



ps aux|grep babi*

# 
# afterinstallbabirl.sh ends here

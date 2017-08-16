#!/bin/bash
# afterinstallbabirl.sh --- 
# 
# Description: 
# Author: Hongyi Wu(吴鸿毅)
# Email: wuhongyi@qq.com 
# Created: 二 8月 15 20:56:42 2017 (+0800)
# Last-Updated: 三 8月 16 15:50:23 2017 (+0800)
#           By: Hongyi Wu(吴鸿毅)
#     Update #: 4
# URL: http://wuhongyi.cn 

#以下需要用户修改
usernamedir="/home/wuhongyi"  #这里wuhongyi为当前用户名

# ------------------------------------------------------------------------------

if [ `whoami` = "root" ];then 
    echo "当前为root用户，能够执行此脚本！" 
else 
    echo "请在ROOT权限下执行此脚本！！！"
    exit 1
fi

# ------------------------------------------------------------------------------

cp ${usernamedir}/babirl/rcd/babimo /etc/init.d  
ln -sf /etc/init.d/babimo /etc/rc2.d/S99babimo
ln -sf /etc/init.d/babimo /etc/rc3.d/S99babimo
ln -sf /etc/init.d/babimo /etc/rc5.d/S99babimo


echo 'PATH=$PATH:'$usernamedir'/babirl/bin/' >> ~/.bashrc



ps aux|grep babi*

# 
# afterinstallbabirl.sh ends here

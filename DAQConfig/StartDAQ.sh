#! /bin/bash
#this is a script to start the DAQ service
#History：
#2013/06/05 First Released    Lee
#2021/12/26 Add root user check   Hongyi Wu

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
    echo "---Start the DAQ service--------------------------"
else
    echo "==ERROR==========================================="
    echo "请在 ROOT 权限下执行本脚本！！！"
    echo "=================================================="
    exit 1
fi

# ------------------------------------------------------------------------------


#kill if any service is run
pkill  babian
pkill  babild
pkill  babinfo
rm -f /home/$1/babirl/run/babian
rm -f /home/$1/babirl/run/babild
rm -f /home/$1/babirl/run/babinfo

#start the DAQ service
/home/$1/babirl/bin/babinfo
/home/$1/babirl/bin/babild -l 10
/home/$1/babirl/bin/babian
ps aux | grep babi

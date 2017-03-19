#! /bin/bash
#this is a script to start the DAQ service
#History：
#2013/06/05 First Released    Lee

#kill if any service is run
pkill  babian
pkill  babild
pkill  babinfo
rm -f /home/wuhongyi/babirl/run/babian
rm -f /home/wuhongyi/babirl/run/babild
rm -f /home/wuhongyi/babirl/run/babinfo

#start the DAQ service
/home/wuhongyi/babirl/bin/babinfo
/home/wuhongyi/babirl/bin/babild -l 10
/home/wuhongyi/babirl/bin/babian
ps aux | grep babi


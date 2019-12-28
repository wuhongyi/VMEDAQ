.. Setting.md --- 
.. 
.. Description: 
.. Author: Hongyi Wu(吴鸿毅)
.. Email: wuhongyi@qq.com 
.. Created: 一 12月  3 10:44:54 2018 (+0800)
.. Last-Updated: 六 12月 28 21:12:29 2019 (+0800)
..           By: Hongyi Wu(吴鸿毅)
..     Update #: 6
.. URL: http://wuhongyi.cn 

=================================
获取配置
=================================

---------------------------------
程序修改建议顺序
---------------------------------

- anaroot/CBLT.hh
- DAQConfig/bbcaenvme/babies/bbmodules.h
- DAQConfig/bbcaenvme/babies/start.c
- DAQConfig/bbcaenvme/babies/evt.c
- DAQConfig/bbcaenvme/babies/clear.c
- DAQConfig/bbcaenvme/babies/stop.c
- DAQConfig/bbcaenvme/init/daqinitrc.sh

----

---------------------------------
V1718/V2718
---------------------------------

V1718/V2718 PCB 板上 DIP 开关：Prog: 0 off, 1 off, 2 off, 3 on, 4 off, I/O NIM

v1718/V2718 前面板 5 个 LEMO 输出口，分别为 0-4

通电时候输出口 0-3 处于高电平，输出口 4 处于低电平。因此软件 BUSY 模式时候采用输出口 4，硬件 BUSY 模式采用输出口 3。


---------------------------------
软件 BUSY 模式
---------------------------------

**待补充**

---------------------------------
硬件 BUSY 模式
---------------------------------

**待补充**

..
.. Setting.md ends here 

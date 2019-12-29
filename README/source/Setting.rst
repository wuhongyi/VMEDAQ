.. Setting.md --- 
.. 
.. Description: 
.. Author: Hongyi Wu(吴鸿毅)
.. Email: wuhongyi@qq.com 
.. Created: 一 12月  3 10:44:54 2018 (+0800)
.. Last-Updated: 日 12月 29 14:43:10 2019 (+0800)
..           By: Hongyi Wu(吴鸿毅)
..     Update #: 8
.. URL: http://wuhongyi.cn 

=================================
获取配置
=================================

---------------------------------
模块安放顺序
---------------------------------

为了方便用户配置 DAQ，这里我们建议用户按照以下顺序依次插入采集模块（如果没有某些类型模块，则调过相应类型的模块）：

- 控制器 V1718/V2718
- 定标器 V830
- V7xx
- V1x90
- MADC32  

---------------------------------
程序修改建议顺序
---------------------------------
- anaroot/CBLT.hh
- DAQConfig/StartDAQ.sh
- DAQConfig/StopDAQ.sh
- DAQConfig/bbcaenvme/babies/bbmodules.h
- DAQConfig/bbcaenvme/babies/start.c
- DAQConfig/bbcaenvme/babies/evt.c
- DAQConfig/bbcaenvme/babies/clear.c
- DAQConfig/bbcaenvme/babies/stop.c
- DAQConfig/bbcaenvme/init/daqinitrc.sh

修改程序，请先仔细阅读 DAQConfig 页面中的说明。
  
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

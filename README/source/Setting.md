<!-- Setting.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 一 12月  3 10:44:54 2018 (+0800)
;; Last-Updated: 一 12月  3 11:04:46 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 3
;; URL: http://wuhongyi.cn -->

# 获取配置

## 程序修改建议顺序

- anaroot/CBLT.hh
- DAQConfig/babies/bbmodules.h
- DAQConfig/babies/start.c
- DAQConfig/babies/evt.c
- DAQConfig/babies/clear.c
- DAQConfig/babies/stop.c
- DAQConfig/init/daqinitrc.sh

----

## V2718

V2718PCB板上DIP开关：Prog: 0 off, 1 off, 2 off, 3 on, 4 off, I/O NIM

V2718前面板5个输出PORT，分别为0-4

通电时候PORT0-3处于高电平，PORT4处于低电平。因此软件BUSY模式时候采用PORT4，硬件BUSY模式采用PORT3。




<!-- Setting.md ends here -->

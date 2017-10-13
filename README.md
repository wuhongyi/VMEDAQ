<!-- README.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 7月  9 15:42:31 2017 (+0800)
;; Last-Updated: 五 10月 13 22:57:51 2017 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 17
;; URL: http://wuhongyi.cn -->

# README

本获取经过Scientific Linux 6/7 系统测试。



文件夹内有以下文件/文件夹：

- analysis（一些用来辅助分析的代码）
- anaroot（底层库，用来将原始数据转为ROOT及在线统计）
- checkcnt（自动检查数据事件关联情况）
- cutpedo（自动拟合推荐合适pedo）
- DAQConfig（获取控制包）
- online（在线监视能量，能谱）
- r2root（数据转换）
- source（babirl源码，将会配置自动化安装脚本）
- statistics(时时监视每路信号的计数率，每10ns更新一次)
- README.md（本文件）

----

## analysis

存放辅助分析程序，当前只放置一个MakeProcess模板。


## anaroot

如果采用CBLT模式读取数据，则先修改*CBLT.hh*文件，不采用CBLT模式则不用修改。设置好之后，执行该目录下的自动编译、安装脚本 *autoPKU.sh* 即可

```bash
sh autoPKU.sh
```


修改 **CBLT.hh** 文件，其中设置应该与CBLT模式下的插件设置顺序一致。

当前CBLT chain支持v830、v7xx、v1190、v1290、madc五种类型的插件，如下所示：

```bash
#define v830m
#define v7xxm
#define v1190m
#define v1290m
#define madcm
```

**获取中如果没有哪一种类型插件，则需注释掉该类型的定义！！！**

以下*xxxn*为启用插件的数据顺序，从0开始编码，如果五种类型插件都有，则为以下设置：
```bash
#define v830n 0
#define v7xxn 1
#define v1190n 2
#define v1290n 3
#define madcn 4
```

如果只含有v7xx、madc两种类型的插件，则定义如下：

```bash
#define v7xxn 0
#define madcn 1
```

如果只含有v830、v7xx、madc三种类型的插件，则定义如下：

```bash
#define v830n 0
#define v7xxn 1
#define madcn 2
```

以下定义用来指定每种类型插件的个数

```bash
#define v830num
#define v7xxnum
#define v1190num
#define v1290num
#define madcnum
```

以下是v830的其它设置

```bash
#define v830chn 8 // 这里设置830开启路数
#define v830head 1 // 不要修改
#define v830geo 0 // 不要修改
```

## checkcnt

用来辅助检查文件中事件是否关联。执行程序之后将会在该文件夹内生成一个pdf文件，检查该文件内每张图数值是否有异常。

## cutpedo

用来辅助设置pedo数值。高斯拟合pedo，并给出三倍sigma的上限作为推荐数值，并生成初始文件夹init内脚本。

## DAQConfig

修改bbcaenvme文件夹下babies、init文件夹内文件

### babies/bbmodules.h

修改ADCADDR、MADCADDR、V1190ADDR、SCAADDR使之与硬件地址匹配（可以多余设置，不可少设置）。其它不要修改。

### babies/start.c

根据文件内提示设置，有该类型插件则开启对应代码，开启对应类型busy代码。其它不要修改。

### babies/evt.c

根据文件内提示设置。其它不要修改。

### babies/clear.c

根据文件内提示设置，有该类型插件则开启对应代码，开启对应类型busy代码。其它不要修改。

### babies/stop.c

根据文件内提示设置，有该类型插件则开启对应代码，开启对应类型busy代码。其它不要修改。

### init/daqinitrc.sh

修改该文件内对应脚本，使之与获取插件对应，用来初始化插件。

**重点是修改 cblt.hh 文件，对启用的插件设置CBLT ADDR 为0xbb，其中MADC还得设置MCST ADDR为0xdd。还得设置每一个插件在CBLT中的顺序，first、mid、last。至少得两个插件才能组成CBLT**


## online

时时监视每路信号的能量信息。

按照提示修改Online.cc文件

图形化界面开发中。。。

----

V2718前面板5个输出PORT，分别为0-4

通电时候PORT0-3处于高电平，PORT4处于低电平。因此软件BUSY模式时候采用PORT4，硬件BUSY模式采用PORT3。


## r2root

仅仅需要修改插件定义即可，无需修改其它代码。

修改文件 UserDefine.hh，按照提示修改即可。 


## source

存放获取驱动、依赖库等以及自动安装脚本。详细内容请阅读文件source/README.md


## statistics

用来监视每路的计数率。


----

## 确定各插件固件版本是否为最新版本

- V1190/V1290
	- Firmware Revision Register(Base Address + 0x1026, read only, D16) 
	- This register contains the firmware revision number coded on 8 bit. 
- madc32
	- 0x600E firmware_revision 

----

## 程序修改建议顺序

- anaroot/CBLT.hh
- DAQConfig/babies/bbmodules.h
- DAQConfig/babies/start.c
- DAQConfig/babies/evt.c
- DAQConfig/babies/clear.c
- DAQConfig/babies/stop.c
- DAQConfig/init/daqinitrc.sh




<!-- README.md ends here -->

<!-- README.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 日 7月  9 15:42:31 2017 (+0800)
;; Last-Updated: 五 9月  1 10:41:49 2017 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 13
;; URL: http://wuhongyi.cn -->

# README


文件夹内有以下文件/文件夹：

- anaroot
- DAQConfig
- online（在线监视）
- r2root（数据转换）
- source（babirl源码，将会配置自动化安装脚本）
- README.md（本文件）

----

## anaroot

如果采用CBLT模式读取数据，则先修改*CBLT.hh*文件，不采用CBLT模式则不用修改。设置好之后，执行该目录下的自动编译、安装脚本 *autoPKU.sh* 即可

```bash
sh autoPKU.sh
```


修改 **CBLT.hh** 文件，其中设置应该与CBLT模式下的插件设置顺序一致。

当前CBLT chain支持v830、v7xx、v1190、madc四种类型的插件，如下所示：

```bash
#define v830m
#define v7xxm
#define v1190m
#define madcm
```

**获取中如果没有哪一种类型插件，则需注释掉该类型的定义！！！**

以下*xxxn*为启用插件的数据顺序，从0开始编码，如果四种类型插件都有，则为以下设置：
```bash
#define v830n 0
#define v7xxn 1
#define v1190n 2
#define madcn 3
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
#define v7xxn
#define v1190n
#define madcn
```

以下是v830的其它设置

```bash
#define v830chn 8 // 这里设置830开启路数
#define v830head 1 // 不要修改
#define v830geo 0 // 不要修改
```


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


## r2root

仅仅需要修改插件定义即可，无需修改其它代码。

修改文件 UserDefine.hh，按照提示修改即可。 

## online

正在优化代码中。。。。


----

V2718前面板5个输出PORT，分别为0-4

通电时候PORT0-3处于高电平，PORT4处于低电平。因此软件BUSY模式时候采用PORT4，硬件BUSY模式采用PORT3。


----

## 确定各插件固件版本是否为最新版本

- V1190/V1290
	- Firmware Revision Register(Base Address + 0x1026, read only, D16) 
	- This register contains the firmware revision number coded on 8 bit. 
- madc32
	- 0x600E firmware_revision 


<!-- README.md ends here -->

<!-- README.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 六 8月  5 12:10:59 2017 (+0800)
;; Last-Updated: 一 12月  3 23:17:55 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 21
;; URL: http://wuhongyi.cn -->

# README

本程序为北京大学实验核物理组当前使用的 VME 获取。

**该获取基于 RIKEN 的获取发展而来。我们已经对原本程序进行较大的修改。如果使用本程序，请严格使用本程序包内的程序，请勿随意升级/替换内部程序/固件。**

----

## 版本

**我们建议用户下载稳定版本**

### 稳定版本

**稳定版本 2018.12.03**  

下载最新版本，请点击:  [VMEDAQ stable](https://github.com/pkuNucExp/VMEDAQ)

网页版说明书请访问:  [说明书](https://pkunucexp.github.io/VMEDAQ/)


### 准预览版本

**准预览版本 2018.12.03**  

程序下载请访问:  [VMEDAQ](https://github.com/wuhongyi/VMEDAQ)

网页版说明书请访问:  [说明书](http://wuhongyi.cn/VMEDAQ/)

----

## 关于

本程序历史维护：
- 李智焕
- 李晶
- 臧宏亮
- 吴鸿毅(wuhongyi@qq.com)


----

## 性能介绍

- 本获取经过Scientific Linux 6/7 系统测试。
- 支持多个机箱同步获取。将插件分散在多个机箱，可大大减少数据传输的死时间。
- 本获取分软件 busy 跟硬件 busy 两种模式。
- 对软件 busy 模式
	- 该模式下，一个事件的死时间由 trigger 门宽，7 us左右模数转换时间，20 us 数据传输中断请求及数据传输时间组成。其中除了数据传输时间，其它三个时间是固定的，大约为 30 us。
	- 限制该模式下计数率的因素为数据传输时间，数据越大，所需传输时间也就越长。
	- 以一个机箱，300-500 路左右输入为例，平均 10000 个触发能够记录 5000-6000 个事件，效率在 50-60%
	- 如果以两三个插件为例，则能够达到 70%+ 以上
- 对硬件 busy 模式
	- 该模下式，一个事件的死时间由 trigger 门宽，7 us 左右模数转换时间两部分组成。
	- 意味着该模式下一个事件的死时间大约在 11 us 左右。
	- 该模式模数转换及数据传输同步进行，因而数据高速传输产生的高频信号会对前放/主放的信号带来微小的影响。
	- 通过适当抬高阈值可消除该影响。
	- 该模式下获取效率及高，平均 10000 个触发能够记录 9000+ 个事件，效率达到90%

----

## 目录

文件夹内有以下文件/文件夹：

- analysis（一些用来辅助分析的代码）
- anaroot（底层库，用来将原始数据转为ROOT及在线统计）
- checkcnt（自动检查数据事件关联情况）
- cutpedo（自动拟合推荐合适pedo）
- DAQConfig（获取控制包）
- firmware（固件）
- httponline（基于网页的在线监视）
- online（在线监视能量，能谱）
- r2root（数据转换）
- source（babirl源码，将会配置自动化安装脚本）
- statistics(时时监视每路信号的计数率，每10ns更新一次)
- README.md（本文件）
- docs(网页版说明书)
- README（md版说明书）
- README.pdf（pdf说明书）





<!-- README.md ends here -->

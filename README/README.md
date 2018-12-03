<!-- README.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 六 8月  5 12:10:59 2017 (+0800)
;; Last-Updated: 一 12月  3 11:21:01 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 20
;; URL: http://wuhongyi.cn -->

# README

本程序为北京大学实验核物理组当前使用的 VME 获取。

**该获取基于 RIKEN 的获取发展而来。我们已经对原本程序进行较大的修改。如果使用本程序，请严格使用本程序包内的程序，请勿随意升级/替换内部程序/固件。**

本程序历史维护：
- 李智焕
- 李晶
- 臧宏亮
- 吴鸿毅(wuhongyi@qq.com)



----

本获取经过Scientific Linux 6/7 系统测试。

支持多个机箱同步获取。将插件分散在多个机箱，可大大减少数据传输的死时间。


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

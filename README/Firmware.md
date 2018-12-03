<!-- Firmware.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 一 12月  3 10:26:33 2018 (+0800)
;; Last-Updated: 一 12月  3 10:41:18 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 1
;; URL: http://wuhongyi.cn -->

# 固件要求

> **[warning] 注意**
>
> 请确保所使用的所有插件固件版本与以下一致。
>
> 我们尽可能保证采用最新的固件。



## 当前固件版本

```text
V2718 FW CONET2 Compliant 	2.14_1.5 	April, 2014
v1190   1.1
MADC32  0224
```

----

## 确定各插件固件版本是否为最新版本

- V1190/V1290
	- Firmware Revision Register(Base Address + 0x1026, read only, D16) 
	- This register contains the firmware revision number coded on 8 bit. 
- madc32
	- 0x600E firmware_revision 

----

**这里补充检查固件版本的操作步骤！！！**


<!-- Firmware.md ends here -->

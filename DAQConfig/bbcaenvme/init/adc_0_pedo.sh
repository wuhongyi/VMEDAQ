#!/bin/sh
BASEADDR=0x1000


####################################
# 以下设置阈值 0x00 - 0xFF
# 当前配置下，该参数为真实阈值除以2
../cmdvme/cmdvme -ww ${BASEADDR}1080	0x37
../cmdvme/cmdvme -ww ${BASEADDR}1082	0x31
../cmdvme/cmdvme -ww ${BASEADDR}1084	0x27
../cmdvme/cmdvme -ww ${BASEADDR}1086	0x39
../cmdvme/cmdvme -ww ${BASEADDR}1088	0x38
../cmdvme/cmdvme -ww ${BASEADDR}108a	0x2e
../cmdvme/cmdvme -ww ${BASEADDR}108c	0x33
../cmdvme/cmdvme -ww ${BASEADDR}108e	0x2d
../cmdvme/cmdvme -ww ${BASEADDR}1090	0x3c
../cmdvme/cmdvme -ww ${BASEADDR}1092	0x38
../cmdvme/cmdvme -ww ${BASEADDR}1094	0x40
../cmdvme/cmdvme -ww ${BASEADDR}1096	0x46
../cmdvme/cmdvme -ww ${BASEADDR}1098	0x3a
../cmdvme/cmdvme -ww ${BASEADDR}109a	0x39
../cmdvme/cmdvme -ww ${BASEADDR}109c	0x39
../cmdvme/cmdvme -ww ${BASEADDR}109e	0x2c
../cmdvme/cmdvme -ww ${BASEADDR}10a0	0x3c
../cmdvme/cmdvme -ww ${BASEADDR}10a2	0x3b
../cmdvme/cmdvme -ww ${BASEADDR}10a4	0x37
../cmdvme/cmdvme -ww ${BASEADDR}10a6	0x35
../cmdvme/cmdvme -ww ${BASEADDR}10a8	0x34
../cmdvme/cmdvme -ww ${BASEADDR}10aa	0x34
../cmdvme/cmdvme -ww ${BASEADDR}10ac	0x41
../cmdvme/cmdvme -ww ${BASEADDR}10ae	0x37
../cmdvme/cmdvme -ww ${BASEADDR}10b0	0x37
../cmdvme/cmdvme -ww ${BASEADDR}10b2	0x3c
../cmdvme/cmdvme -ww ${BASEADDR}10b4	0x3a
../cmdvme/cmdvme -ww ${BASEADDR}10b6	0x26
../cmdvme/cmdvme -ww ${BASEADDR}10b8	0x33
../cmdvme/cmdvme -ww ${BASEADDR}10ba	0x30
../cmdvme/cmdvme -ww ${BASEADDR}10bc	0x42
../cmdvme/cmdvme -ww ${BASEADDR}10be	0x3b

#第一个插件ch0设置为0
../cmdvme/cmdvme -ww ${BASEADDR}1080    0x0	

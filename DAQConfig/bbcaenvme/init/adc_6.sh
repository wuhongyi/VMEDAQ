#!/bin/sh
cmd="../cmdvme/cmdvme"
BASEADDR=0x1006

###用户可能需要修改的参数为BASEADDR/GEO,阈值设置在pedo文件

## GEO
# 0x1002, read/write / This register contains the geographical address of the module. The register is filled up upon arrival of a RESET.
${cmd} -ww ${BASEADDR}1002 6  # 0-31

## SW reset
#0x1016 write only / A write access to this dummy register performs a module reset. This register must be used very carefully and for debugging purposes only. 
#${cmd} -ww ${BASEADDR}1016 0x1
#0x1006 Bit Set 1 Register
${cmd} -ww ${BASEADDR}1006 0x80 #Sets the module to a permanent RESET status. 
#0x1008 Bit Clear 1 Register
${cmd} -ww ${BASEADDR}1008 0x80

#在 cblt.sh 文件中设置
# CBLT/MCST ADDR
# Default (at power on ) 0xaa
#${cmd} -ww ${BASEADDR}1004 0xbb

## BERR and BLKEND
#0x1010, read/write / Control Register 1
# only BLKEND enabled
#${cmd} -ww ${BASEADDR}1010 0x4
# obly BERR enabled
#${cmd} -ww ${BASEADDR}1010 0x20
# BERR and BLKEND enabled
${cmd} -ww ${BASEADDR}1010 0x24

# MCST/CBLT control 
# 0x0 DISABLE
# 0x1 LAST BOARD
# 0x2 FIRST BOARD
# 0x3 MID BOARD
${cmd} -ww ${BASEADDR}101a 0x1



#0x1032 Bit Set 2 Register
#0x1034 Bit Clear 2 Register
# OVER RANGE SUPPERSSION DISABLE : all the data are written into the output buffer
# STEP TH =2 / Another option is 16
# x2 0-510    x16 0-4080
${cmd} -ww ${BASEADDR}1032 0x108

# LOW THRESHOLD enable
${cmd} -ww ${BASEADDR}1034 0x10 #only data above the threshold are written into the output buffer

# write header and ender when there is no accept evt
${cmd} -ww ${BASEADDR}1032 0x1000 #when there are no accepted channels, the Header and the EOB are anyway written in the output buffer.

####################################

${cmd} -ww ${BASEADDR}1080	0x00
${cmd} -ww ${BASEADDR}1082	0x00
${cmd} -ww ${BASEADDR}1084	0x00
${cmd} -ww ${BASEADDR}1086	0x00
${cmd} -ww ${BASEADDR}1088	0x00
${cmd} -ww ${BASEADDR}108a	0x00
${cmd} -ww ${BASEADDR}108c	0x00
${cmd} -ww ${BASEADDR}108e	0x00
${cmd} -ww ${BASEADDR}1090	0x00
${cmd} -ww ${BASEADDR}1092	0x00
${cmd} -ww ${BASEADDR}1094	0x00
${cmd} -ww ${BASEADDR}1096	0x00
${cmd} -ww ${BASEADDR}1098	0x00
${cmd} -ww ${BASEADDR}109a	0x00
${cmd} -ww ${BASEADDR}109c	0x00
${cmd} -ww ${BASEADDR}109e	0x00
${cmd} -ww ${BASEADDR}10a0	0x00
${cmd} -ww ${BASEADDR}10a2	0x00
${cmd} -ww ${BASEADDR}10a4	0x00
${cmd} -ww ${BASEADDR}10a6	0x00
${cmd} -ww ${BASEADDR}10a8	0x00
${cmd} -ww ${BASEADDR}10aa	0x00
${cmd} -ww ${BASEADDR}10ac	0x00
${cmd} -ww ${BASEADDR}10ae	0x00
${cmd} -ww ${BASEADDR}10b0	0x00
${cmd} -ww ${BASEADDR}10b2	0x00
${cmd} -ww ${BASEADDR}10b4	0x00
${cmd} -ww ${BASEADDR}10b6	0x00
${cmd} -ww ${BASEADDR}10b8	0x00
${cmd} -ww ${BASEADDR}10ba	0x00
${cmd} -ww ${BASEADDR}10bc	0x00
${cmd} -ww ${BASEADDR}10be	0x00

#!/bin/sh
cmd="../cmdvme/cmdvme"
BASEADDR=0x1000

# GEO 
${cmd} -ww ${BASEADDR}1002 0

# SW reset
#${cmd} -ww ${BASEADDR}1016 0x1
${cmd} -ww ${BASEADDR}1006 0x80
${cmd} -ww ${BASEADDR}1008 0x80

# CBLT/MCST ADDR
# Default (at power on ) 0xaa
${cmd} -ww ${BASEADDR}1004 0xbb

# BERR and BLKEND
# only BLKEND enabled
#${cmd} -ww ${BASEADDR}1010 0x1
# obly BERR enabled
#${cmd} -ww ${BASEADDR}1010 0x20
# BERR and BLKEND enabled
${cmd} -ww ${BASEADDR}1010 0x21

# MCST/CBLT control 
# 0x0 DISABLE
# 0x1 LAST BOARD
# 0x2 FIRST BOARD
# 0x3 MID BOARD
${cmd} -ww ${BASEADDR}101a 0x1

# OVER RANGE SUPPERSSION DISABLE
# STEP TH =2 / Another option is 16
${cmd} -ww ${BASEADDR}1032 0x108

# LOW THRESHOLD enable
${cmd} -ww ${BASEADDR}1034 0x10

# write header and ender when there is no accept evt
${cmd} -ww ${BASEADDR}1032 0x1000

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

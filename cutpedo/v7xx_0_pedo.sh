#!/bin/sh
cmd="../cmdvme/cmdvme"
BASEADDR=0x1000


####################################

# 以下设置阈值 0x00 - 0xFF
# 当前配置下，该参数为真实阈值除以2
${cmd} -ww ${BASEADDR}1080	0x38
${cmd} -ww ${BASEADDR}1082	0x38
${cmd} -ww ${BASEADDR}1084	0x2c
${cmd} -ww ${BASEADDR}1086	0x1d
${cmd} -ww ${BASEADDR}1088	0x27
${cmd} -ww ${BASEADDR}108a	0x2e
${cmd} -ww ${BASEADDR}108c	0x34
${cmd} -ww ${BASEADDR}108e	0x22
${cmd} -ww ${BASEADDR}1090	0x2d
${cmd} -ww ${BASEADDR}1092	0x2a
${cmd} -ww ${BASEADDR}1094	0x49
${cmd} -ww ${BASEADDR}1096	0x2d
${cmd} -ww ${BASEADDR}1098	0x3f
${cmd} -ww ${BASEADDR}109a	0x3f
${cmd} -ww ${BASEADDR}109c	0x3f
${cmd} -ww ${BASEADDR}109e	0x34
${cmd} -ww ${BASEADDR}10a0	0x47
${cmd} -ww ${BASEADDR}10a2	0x2f
${cmd} -ww ${BASEADDR}10a4	0x3c
${cmd} -ww ${BASEADDR}10a6	0x3a
${cmd} -ww ${BASEADDR}10a8	0x36
${cmd} -ww ${BASEADDR}10aa	0x49
${cmd} -ww ${BASEADDR}10ac	0x3c
${cmd} -ww ${BASEADDR}10ae	0x4b
${cmd} -ww ${BASEADDR}10b0	0x36
${cmd} -ww ${BASEADDR}10b2	0x3a
${cmd} -ww ${BASEADDR}10b4	0x3c
${cmd} -ww ${BASEADDR}10b6	0x34
${cmd} -ww ${BASEADDR}10b8	0x48
${cmd} -ww ${BASEADDR}10ba	0x40
${cmd} -ww ${BASEADDR}10bc	0x34
${cmd} -ww ${BASEADDR}10be	0x41

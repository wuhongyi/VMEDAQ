#!/bin/sh
cmd="../cmdvme/cmdvme -bd V2718" #V1718/V2718
BASEADDR=0x1003


####################################

${cmd} -ww ${BASEADDR}1080	0x46
${cmd} -ww ${BASEADDR}1082	0x4a
${cmd} -ww ${BASEADDR}1084	0x3b
${cmd} -ww ${BASEADDR}1086	0x3f
${cmd} -ww ${BASEADDR}1088	0x3e
${cmd} -ww ${BASEADDR}108a	0x4d
${cmd} -ww ${BASEADDR}108c	0x4d
${cmd} -ww ${BASEADDR}108e	0x47
${cmd} -ww ${BASEADDR}1090	0x45
${cmd} -ww ${BASEADDR}1092	0x3e
${cmd} -ww ${BASEADDR}1094	0x3f
${cmd} -ww ${BASEADDR}1096	0x4a
${cmd} -ww ${BASEADDR}1098	0x4b
${cmd} -ww ${BASEADDR}109a	0x4e
${cmd} -ww ${BASEADDR}109c	0x49
${cmd} -ww ${BASEADDR}109e	0x44
${cmd} -ww ${BASEADDR}10a0	0x56
${cmd} -ww ${BASEADDR}10a2	0x48
${cmd} -ww ${BASEADDR}10a4	0x4a
${cmd} -ww ${BASEADDR}10a6	0x52
${cmd} -ww ${BASEADDR}10a8	0x47
${cmd} -ww ${BASEADDR}10aa	0x42
${cmd} -ww ${BASEADDR}10ac	0x3c
${cmd} -ww ${BASEADDR}10ae	0x4e
${cmd} -ww ${BASEADDR}10b0	0x3e
${cmd} -ww ${BASEADDR}10b2	0x3c
${cmd} -ww ${BASEADDR}10b4	0x3f
${cmd} -ww ${BASEADDR}10b6	0x41
${cmd} -ww ${BASEADDR}10b8	0x55
${cmd} -ww ${BASEADDR}10ba	0x4a
${cmd} -ww ${BASEADDR}10bc	0x43
${cmd} -ww ${BASEADDR}10be	0x47

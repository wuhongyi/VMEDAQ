#!/bin/sh
cmd="../cmdvme/cmdvme"
BASEADDR=0x2001


####################################

${cmd} -ww ${BASEADDR}4000	0x2f
${cmd} -ww ${BASEADDR}4002	0x29
${cmd} -ww ${BASEADDR}4004	0x2a
${cmd} -ww ${BASEADDR}4006	0x2b
${cmd} -ww ${BASEADDR}4008	0x2a
${cmd} -ww ${BASEADDR}400a	0x2a
${cmd} -ww ${BASEADDR}400c	0x2b
${cmd} -ww ${BASEADDR}400e	0x2b
${cmd} -ww ${BASEADDR}4010	0x29
${cmd} -ww ${BASEADDR}4012	0x2a
${cmd} -ww ${BASEADDR}4014	0x28
${cmd} -ww ${BASEADDR}4016	0x2b
${cmd} -ww ${BASEADDR}4018	0x2a
${cmd} -ww ${BASEADDR}401a	0x29
${cmd} -ww ${BASEADDR}401c	0x2a
${cmd} -ww ${BASEADDR}401e	0x2b
${cmd} -ww ${BASEADDR}4020	0x29
${cmd} -ww ${BASEADDR}4022	0x29
${cmd} -ww ${BASEADDR}4024	0x29
${cmd} -ww ${BASEADDR}4026	0x28
${cmd} -ww ${BASEADDR}4028	0x28
${cmd} -ww ${BASEADDR}402a	0x28
${cmd} -ww ${BASEADDR}402c	0x2a
${cmd} -ww ${BASEADDR}402e	0x29
${cmd} -ww ${BASEADDR}4030	0x2a
${cmd} -ww ${BASEADDR}4032	0x29
${cmd} -ww ${BASEADDR}4034	0x29
${cmd} -ww ${BASEADDR}4036	0x28
${cmd} -ww ${BASEADDR}4038	0x29
${cmd} -ww ${BASEADDR}403a	0x28
${cmd} -ww ${BASEADDR}403c	0x29
${cmd} -ww ${BASEADDR}403e	0x2a

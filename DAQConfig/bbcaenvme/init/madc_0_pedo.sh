#!/bin/sh
cmd="../cmdvme/cmdvme"
BASEADDR=0x2000


####################################

${cmd} -ww ${BASEADDR}4000	0xFF
${cmd} -ww ${BASEADDR}4002	0xFF
${cmd} -ww ${BASEADDR}4004	0xFF
${cmd} -ww ${BASEADDR}4006	0xFF
${cmd} -ww ${BASEADDR}4008	0x0F
${cmd} -ww ${BASEADDR}400a	0xFF
${cmd} -ww ${BASEADDR}400c	0xFF
${cmd} -ww ${BASEADDR}400e	0xFF
${cmd} -ww ${BASEADDR}4010	0x0F
${cmd} -ww ${BASEADDR}4012	0xFF
${cmd} -ww ${BASEADDR}4014	0xFF
${cmd} -ww ${BASEADDR}4016	0xFF
${cmd} -ww ${BASEADDR}4018	0xFF
${cmd} -ww ${BASEADDR}401a	0x0F
${cmd} -ww ${BASEADDR}401c	0xFF
${cmd} -ww ${BASEADDR}401e	0xFF
${cmd} -ww ${BASEADDR}4020	0xFF
${cmd} -ww ${BASEADDR}4022	0xFF
${cmd} -ww ${BASEADDR}4024	0xFF
${cmd} -ww ${BASEADDR}4026	0x0F
${cmd} -ww ${BASEADDR}4028	0xFF
${cmd} -ww ${BASEADDR}402a	0xFF
${cmd} -ww ${BASEADDR}402c	0xFF
${cmd} -ww ${BASEADDR}402e	0xFF
${cmd} -ww ${BASEADDR}4030	0xFF
${cmd} -ww ${BASEADDR}4032	0xFF
${cmd} -ww ${BASEADDR}4034	0xFF
${cmd} -ww ${BASEADDR}4036	0xFF
${cmd} -ww ${BASEADDR}4038	0xFF
${cmd} -ww ${BASEADDR}403a	0xFF
${cmd} -ww ${BASEADDR}403c	0xFF
${cmd} -ww ${BASEADDR}403e	0xFF

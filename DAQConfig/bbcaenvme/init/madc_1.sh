#!/bin/sh
cmd="../cmdvme/cmdvme"
BASEADDR=0x2001

# SW reset
${cmd} -ww ${BASEADDR}6008 0x1

# Stop madc acceptting gate signals
${cmd} -ww ${BASEADDR}603A 0

# GEO 
${cmd} -ww ${BASEADDR}6004 11

# Input Range 
# 0 -> 4V (default)
# 1 -> 10V
# 2 -> 8V
${cmd} -ww ${BASEADDR}6060 2

# Marking Type
# 0-> Event Counter
# 1-> time stamp
# 2-> extended time stamp
${cmd} -ww ${BASEADDR}6038 0

# ADC RESOLUTION
#    0 -> 2k (800ns convesion time)
#    1 -> 4k (1.6us conversion time)
#    2 -> 4k hires (3.2us conversion time)
#    3 -> 8k (6.4us conversion time)
#    4 -> 8k hires (12.5us conv. Time)
${cmd} -ww ${BASEADDR}6042 4

# enable/disbale thresholds
# 0 enable (default)
# 1 disbale
${cmd} -ww ${BASEADDR}604C 0

# set multi event register
# bit [1..0] 
# 0(default), no multievent buffering, deny all gate signal until 0x6034 clears event
# 1, no berr signal
# 3, multievent buffering, after the number of readout words 
# exceeds the number specified by 0x601A, the next end of event mark terminates
# transfer by emitting BERR.
# bit [2] 1: EOB 0: BERR(default)
${cmd} -ww ${BASEADDR}6036 0x3
# default 1 
${cmd} -ww ${BASEADDR}601A 0x1

# disable IRQ 
# 0 (default) IRQ disbaled
# [1..7] IRQ enable and IRQ priority
${cmd} -ww ${BASEADDR}6010 0x0

# set nim4 output 
# 0x0:busy;0x1:gate0 output;0x2:gate1 output; 
#${cmd} -ww ${BASEADDR}606E 0x1

# CBLT MCST control register 0x6020 default 0x0
# --------------------------------------------------------------------------
# bit [ 7 6 5 4 3 2 1 0 ]
#	W					R
# 0	1 Disable CBLT				1 CBLT enabled
#	0 no effect				0 CBLT disabled
# 1	1 Enable CBLT          	                *
#	0 no effect				*
# 2	1 Disable last module in CBLT Chain     1 last module in chain
# 	0 no effect				0 not last module in chain
# 3	1 enbale last module in CBLT chain	*
# 	0 no effect 				*
# 4	1 disable 1st module in CBLT chain	1 1st module in chain
# 	0 no effect				0 not 1st in chain
# 5	1 enbale 1st module in CBLT chain	*
# 	0 no effect				*
# 6	1 disable MCST				1 MCST enabled
#	0 no effect				0 MCST disabled
# 7	1 enable MCST				*
# 	0 no effect
# ---------------------------------------------------------------------------
# enable CBLT
#${cmd} -ww ${BASEADDR}6020  0x2
# enable last module 0x8
# enable 1st module 0x20
# default mid
#${cmd} -ww ${BASEADDR}6020  0x8
# enable MCST 
#${cmd} -ww ${BASEADDR}6020  0x40
# CBLT ADDR 0xAA (default)
# 0xAA001234
#${cmd} -ww ${BASEADDR}6022  0xbb
# MCST ADDR 0xbb (default)
# 0xBB001234
#${cmd} -ww ${BASEADDR}6024  0xbb

# Lower threshold
${cmd} -ww ${BASEADDR}4000 0
${cmd} -ww ${BASEADDR}4002 0
${cmd} -ww ${BASEADDR}4004 0
${cmd} -ww ${BASEADDR}4006 0
${cmd} -ww ${BASEADDR}4008 0
${cmd} -ww ${BASEADDR}400a 0
${cmd} -ww ${BASEADDR}400c 0
${cmd} -ww ${BASEADDR}400e 0
${cmd} -ww ${BASEADDR}4010 0
${cmd} -ww ${BASEADDR}4012 0
${cmd} -ww ${BASEADDR}4014 0
${cmd} -ww ${BASEADDR}4016 0
${cmd} -ww ${BASEADDR}4018 0
${cmd} -ww ${BASEADDR}401a 0
${cmd} -ww ${BASEADDR}401c 0
${cmd} -ww ${BASEADDR}401e 0
${cmd} -ww ${BASEADDR}4020 0
${cmd} -ww ${BASEADDR}4022 0
${cmd} -ww ${BASEADDR}4024 0
${cmd} -ww ${BASEADDR}4026 0
${cmd} -ww ${BASEADDR}4028 0
${cmd} -ww ${BASEADDR}402a 0
${cmd} -ww ${BASEADDR}402c 0
${cmd} -ww ${BASEADDR}402e 0
${cmd} -ww ${BASEADDR}4030 0
${cmd} -ww ${BASEADDR}4032 0
${cmd} -ww ${BASEADDR}4034 0
${cmd} -ww ${BASEADDR}4036 0
${cmd} -ww ${BASEADDR}4038 0
${cmd} -ww ${BASEADDR}403a 0
${cmd} -ww ${BASEADDR}403c 0
${cmd} -ww ${BASEADDR}403e 0

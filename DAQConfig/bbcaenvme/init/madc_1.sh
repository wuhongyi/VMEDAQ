#!/bin/sh
cmd="../cmdvme/cmdvme -bd V2718" #V1718/V2718
BASEADDR=0x2001

# SW reset
#breaks all activities, sets critical parameters to default
${cmd} -ww ${BASEADDR}6008 0x1

# Stop madc acceptting gate signals
#1 -> start accepting gates
#If no external trigger logic, which stops the gates when daq is not running, is implemented, this register should be set to 0 be-fore applying the FIFO_reset to get a well de-fined status. When setting it to 1 again for data acquisition start, the buffer is in a well defined status.
${cmd} -ww ${BASEADDR}603A 0


# GEO 
${cmd} -ww ${BASEADDR}6004 11  #0-31

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
# 1 disbale  ->  threshold settings at 0x4000.. are ignored (assumed as 0)
${cmd} -ww ${BASEADDR}604C 0


# set multi event register
# bit [1..0] 
# 0(default), no multievent buffering, deny all gate signal until 0x6034 clears event
# 1, no berr signal
# 3, multievent buffering, after the number of readout words exceeds the number specified by 0x601A, the next end of event mark terminates transfer by emitting BERR.
# bit [2] 1: EOB 0: BERR(default)
#最佳选择：软件BUSY模式选择0x1，不需要Clear。硬件BUSY选择0x3，需要Clear。
${cmd} -ww ${BASEADDR}6036 0x3


# Max_transfer_data   default 1
#Maximum data words to transfer before ending the transfer at next end of event word. Only works for multi event mode 3. At Max_transfer_data = 1, 1 event per transfer is emitted. Maximum number of events is 2047. Usually the same or higher value than in 0x6018 is used. Setting the value to 0 allows unlimited transfer.
${cmd} -ww ${BASEADDR}601A 0x1


# disable IRQ 
# 0 (default) IRQ disbaled
# [1..7] IRQ enable and IRQ priority
${cmd} -ww ${BASEADDR}6010 0x0



#ECL term (default:0)
#switch ECL terminators on (1= on) low bit for: “gate0”, high bit for "fc"
#Switch terminators off when inputs are not used. 
#Then inputs will be set to a well defined state by internal weak resistors.
#${cmd} -ww ${BASEADDR}6062 0x0

#ECL G1 (default:0)
#0: gate1 input, 
#1: oscillator input(also set 0x6096 !!)
#${cmd} -ww ${BASEADDR}6064 0x0

#ECL FC (default:0)
#0: fast clear input,  
#1: reset time stamp oscillator input
#${cmd} -ww ${BASEADDR}6066 0x0

#ECL BY (default:0)
#0: as busy output,
#1: reserved 
#${cmd} -ww ${BASEADDR}6068 0x0


#NIM Gate1/Osc (default:0)
#0: gate1 input, 
#1: oscillator input(also set 0x6096 !!) 
#${cmd} -ww ${BASEADDR}606A 0x0

#NIM FC/Res (default:0)
#0: fast clear input,  
#1: reset time stamp oscillator, hold at value 0
#${cmd} -ww ${BASEADDR}606C 0x0

#NIM Busy/Cbus (default:0)
#0: as busy (in independent bank operation or toggle mode: active when both banks are busy) 
#1: as gate0 output
#2: as gate1 output
#3: as Cbus output(needs up to 100 us. Read signals the correct status)
#4: buffer full
#8: data in buffer above threshold 0x6018
#9: events in buffer above threshold 0x601E
#${cmd} -ww ${BASEADDR}606E 0x1


# --------------------------------------------------------------------------

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

#!/bin/sh

cmd="../cmdvme/cmdvme"
BASEADDR=0x4000
opt=""


echo "load tdc setting ..."

# set geometry
#../cmdvme/cmdvme -ww ${BASEADDR}100e 0xa
${cmd} $opt -ww ${BASEADDR}100e 20
sleep 0

# Hardware Reset
${cmd} $opt -ww ${BASEADDR}1014 1
sleep 0

# Software Reset
${cmd} $opt -ww ${BASEADDR}1016 1
sleep 0


#0x1000, read/write, D16 / Control Register
# compansation enbale(0x0020)/disable(0x0000)
# empty evt (0x0008)      : Write global header when there are no data
# event fifo ena (0x0100) : Can read word count in one event 
# BERR enable (0x0001)    : User BERR fro Block transfer
# comp. disable + empty evt + event fifo ena = 0x0108
# comp. enable + empty evt + event fifo ena = 0x0128 
# comp. enable + empty evt + event fifo ena = 0x0029 for BERR readout
#${cmd} $opt -ww ${BASEADDR}1000 0x0100
#${cmd} $opt -ww ${BASEADDR}1000 0x0128
${cmd} $opt -ww ${BASEADDR}1000 0x0029
sleep 1

# Interrupt Level = 1  --> startup
${cmd} $opt -ww ${BASEADDR}100a 0
sleep 0

# Almost Full Level //5 is good value
# 3 is also ok (may have firmware dependnce)
${cmd} $opt -ww ${BASEADDR}1022 5
sleep 1


# Set Triger Matching Mode
echo "set trigger matting mode..."
${cmd} $opt -qn ${BASEADDR}0000 0x0000
sleep 1
echo "done"

# Set Window Width (1=25ns, 16=400ns)
# 10.5us = 10500ns = 420
# 15.5us = 15500ns = 620
# 40.5us = 15500ns = 1620
${cmd} $opt -qw ${BASEADDR}0000 0x1000 240
sleep 1

##以下参数为上面数值加8然后取相反数
# Set Window Offset (-2048 to +40, 1=25ns, -4=-100ns)
# -4us  = -4000ns  = -160
# -10us = -10000ns = -400
# -15us = -15000ns = -600
# -40us = -15000ns = -1600
${cmd} $opt -qw ${BASEADDR}0000 0x1100 -248
sleep 1

# Set Edge Detection (1=trailing, 2=leading, 3=both)
${cmd} $opt -qw ${BASEADDR}0000 0x2200 2
sleep 1

# Set Extra Search Margin (1=25ns, default=8)
${cmd} $opt -qw ${BASEADDR}0000 0x1200 8
sleep 1

# Set Reject Margin (1=25ns, default=4)
${cmd} $opt -qw ${BASEADDR}0000 0x1300 4
sleep 1

# Enable Subtraction of Trigger Time
${cmd} $opt -qn ${BASEADDR}0000 0x1400
sleep 1

# disable TDC Header, Trailer
#${cmd} $opt -qn ${BASEADDR}0000 0x3100
#sleep 1

# enable TDC Header, Trailer
${cmd} $opt -qn ${BASEADDR}0000 0x3000
sleep 0

# read only 1 event from 1 CBLT cycle
${cmd} $opt -ww ${BASEADDR}1024 0x1

## Module Software clear
#${cmd} $opt -ww ${BASEADDR}1016 1
#sleep 0

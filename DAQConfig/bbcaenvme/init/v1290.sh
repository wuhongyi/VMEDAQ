#!/bin/sh

cmdvme="../cmdvme/cmdvme"
BASEADDR=0x4001
opt="-bd V2718" #V1718/V2718

echo "load tdc setting ..."

# set geometry
#0x100E, read/write, D16
$cmdvme $opt -ww ${BASEADDR}100e 21   #0-31
#N.B.: The VME std. versions (V1290 A/N) use this register ONLY for data identification during CBLT operation.
sleep 0





# Module Reset Register
#0x1014 write only, D16
#A dummy access to this register allows to generate a single shot RESET of the module.
$cmdvme $opt -ww ${BASEADDR}1014 1
sleep 0

# Software Clear Register
#0x1016 write only, D16
#A write access to this location causes the following: the TDCs are cleared; the Output Buffer is cleared; the Event counter is set to 0; TDC Global Reset
$cmdvme $opt -ww ${BASEADDR}1016 1
sleep 0


#0x1000, read/write, D16 / Control Register
#This register allows performing some general settings of the module.
# compansation enbale(0x0020)/disable(0x0000)
# empty evt (0x0008)      : Write global header when there are no data
# event fifo ena (0x0100) : Can read word count in one event 
# BERR enable (0x0001)    : User BERR fro Block transfer
# comp. disable + empty evt + event fifo ena = 0x0108
# comp. enable + empty evt + event fifo ena = 0x0128 
# comp. enable + empty evt + event fifo ena = 0x0029 for BERR readout
$cmdvme $opt -ww ${BASEADDR}1000 0x0029
sleep 1

# Interrupt Level = 1  --> startup
#0x100A, read/write, D16
#The 3 LSB of this register contain the value of the interrupt level (Bits 3 to 15 are meaningless). Default setting is 0x0. In this case interrupt generation is disabled.
$cmdvme $opt -ww ${BASEADDR}100a 0
sleep 0

# Almost Full Level //5 is good value
# 3 is also ok (may have firmware dependnce)
$cmdvme $opt -ww ${BASEADDR}1022 5
sleep 1


# Set Triger Matching Mode
echo "set trigger matting mode..."
$cmdvme -qn ${BASEADDR}0000 0x0000 $opt # Automatic_Reject and keep_token are automatically enabled 
sleep 1
echo "done"


# Set Window Width (1=25ns, 16=400ns)
# 10.5us = 10500ns = 420
# 15.5us = 15500ns = 620
# 40.5us = 15500ns = 1620
#It allows to set the width of the match window. After this OPCODE is sent, a 16-bit word must be written at the same location of the OPCODE itself. The microcontroller will remain in a wait status until this 16-bit word is written. The value of the word can be set in a range from 1 to 2088 (significant bits: [0;11]), or hex FFF, taking into account the Timing Constraints: the relevant window width is the word value times the clock period (25 ns).
#Default setting: 0x14 -> 500 ns
$cmdvme -qw ${BASEADDR}0000 0x1000 240 $opt
sleep 1

# Set Window Offset (-2048 to +40, 1=25ns, -4=-100ns)
# -10us = -10000ns = -400
# -15us = -15000ns = -600
# -40us = -15000ns = -1600
#It allows to set the offset of the match window with respect to the trigger itself, i.e. the time difference (expressed in clock cycles, 1 cycle = 25 ns) between the start of the match window and the trigger time. After this OPCODE is sent, a 16-bit word must be written at the same location of the OPCODE itself (the window offset is a 12 bit signed number; the transfer on the VME bus takes place on 16 bit, therefore the sign bit must be extended to the full word in order to avoid mistakes). The microcontroller will remain in a wait status until a 16-bit word is written. The window offset value must be set in the range from -2048 (hex 0xF800) to +40 (hex 0x0028). The offset and width value must be set according to the constraints described in § 0. The window offset is synchronised with the clock cycle, thus there could be a jitter of one clock cycle in the actual offset position.
#Default setting: 0xFFD8 -> -1 μs
$cmdvme -qw ${BASEADDR}0000 0x1100 -248 $opt
sleep 1

# Set Edge Detection (1=trailing, 2=leading, 3=both)
#It allows to set the TDCs’ edge detection. After this OPCODE is sent, a 16-bit word must be written at the MICRO register address. The microcontroller will remain in a wait status until a 16 bit word is written. This word’s two LSBs have the following meaning: 00 -> pair mode (available only in Lower resolutions modes); 01 -> only trailing; 10 -> only leading; 11 -> trailing and leading.
$cmdvme -qw ${BASEADDR}0000 0x2200 2 $opt
sleep 1

# Set Extra Search Margin (1=25ns, default=8)
#It allows to set the extra search field of the match window. After this OPCODE is sent, a 16-bit word must be written at the same location of the OPCODE itself. The microcontroller will remain in a wait status until a 16-bit word is written. The margin value (clock cycles) can be any 12 bit value (bits [12;15] are meaningless), though reasonable values are not greater than 50.
#Default setting: 0x08 -> 200 ns
$cmdvme -qw ${BASEADDR}0000 0x1200 8 $opt
sleep 1


# Set Reject Margin (1=25ns, default=4)
#It allows to set the reject margin, expressed in clock cycles. After this OPCODE is sent, a 16-bit word must be written at the same location of the OPCODE itself. The microcontroller will remain in a wait status until a 16-bit word is written. The margin value can be any 12 bit value (bits [12:15] are meaningless), 0 sets the margin at the beginning of the match window; it is recommended to set the margin ≥ 1.
#Default setting: 0x04 -> 100 ns
$cmdvme -qw ${BASEADDR}0000 0x1300 4 $opt
sleep 1

# Enable Subtraction of Trigger Time
#It allows to enable the trigger time tag subtraction: in this operating mode the time measurements are referred to the beggining of the match window.
$cmdvme -qn ${BASEADDR}0000 0x1400 $opt
sleep 1

# disable TDC Header, Trailer
#It allows to disable the TDCs’ Header and Trailer during data readout.
#$cmdvme $opt -qn 0x40000000 0x3100
#sleep 1

# enable TDC Header, Trailer
#It allows to enable the TDCs’ Header and Trailer during data readout.
$cmdvme -qn ${BASEADDR}0000 0x3000 $opt
sleep 0


# BLT Event Number Register
#0x1024, read/write, D16
#This register contains the number Ne of complete events which is desirable to transfer via BLT. The number of events must be written in a 8 bit word. The Register’s default setting is 0, which means that the Event Aligned BLT is disabled. This Register must be accessed in D16 mode. 
# read only 1 event from 1 CBLT cycle
${cmdvme} -ww ${BASEADDR}1024 0x1 $opt

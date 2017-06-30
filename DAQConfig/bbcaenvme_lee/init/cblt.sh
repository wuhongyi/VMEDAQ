#!/bin/sh

#for ADCS

# enable BERR and BLKEND
../cmdvme/cmdvme -ww 0x10001010 0x24
../cmdvme/cmdvme -ww 0x10011010 0x24
../cmdvme/cmdvme -ww 0x10021010 0x24
../cmdvme/cmdvme -ww 0x10031010 0x24
../cmdvme/cmdvme -ww 0x10041010 0x24
../cmdvme/cmdvme -ww 0x10051010 0x24
#./cmdvme/cmdvme -ww 0x10061010 0x24

# CBLT/MCST ADDR
# Default (at power on ) 0xaa
../cmdvme/cmdvme -ww 0x10001004 0xbb
../cmdvme/cmdvme -ww 0x10011004 0xbb
../cmdvme/cmdvme -ww 0x10021004 0xbb
../cmdvme/cmdvme -ww 0x10031004 0xbb
../cmdvme/cmdvme -ww 0x10041004 0xbb
../cmdvme/cmdvme -ww 0x10051004 0xbb
#../cmdvme/cmdvme -ww 0x10061004 0xbb

# CBLT/MCST ADDR For v830AC
#../cmdvme/cmdvme -ww 0x5000111c 0xbb

# CBLT/MCST ADDR For v1190AC
# ../cmdvme/cmdvme -ww 0x40001010 0xbb
# ../cmdvme/cmdvme -ww 0x40011010 0xbb

# CBLT ADDR For MADC32
# ../cmdvme/cmdvme -ww 0x20006022 0xbb
# ../cmdvme/cmdvme -ww 0x20016022 0xbb
# ../cmdvme/cmdvme -ww 0x20026022 0xbb
# ../cmdvme/cmdvme -ww 0x20036022 0xbb
# ../cmdvme/cmdvme -ww 0x20046022 0xbb
# ../cmdvme/cmdvme -ww 0x20056022 0xbb
# ../cmdvme/cmdvme -ww 0x20066022 0xbb

# MCST ADDR For MADC32
# ../cmdvme/cmdvme -ww 0x20006024 0xdd
# ../cmdvme/cmdvme -ww 0x20016024 0xdd
# ../cmdvme/cmdvme -ww 0x20026024 0xdd
# ../cmdvme/cmdvme -ww 0x20036024 0xdd
# ../cmdvme/cmdvme -ww 0x20046024 0xdd
# ../cmdvme/cmdvme -ww 0x20056024 0xdd
# ../cmdvme/cmdvme -ww 0x20066024 0xdd

# ******************
# MCST/CBLT control 
# For CAEN Modules
# 0x0 INACTIVE
# 0x1 LAST BOARD
# 0x2 FIRST BOARD
# 0x3 MID BOARD
# ******************
# For MADC32
# 0xA2 1st
# 0x82 mid
# 0x8A last 
# ******************
# ../cmdvme/cmdvme -ww 0x5000111e 0x2

../cmdvme/cmdvme -ww 0x1000101a 0x2
../cmdvme/cmdvme -ww 0x1001101a 0x3
../cmdvme/cmdvme -ww 0x1002101a 0x3
../cmdvme/cmdvme -ww 0x1003101a 0x3
../cmdvme/cmdvme -ww 0x1004101a 0x3
../cmdvme/cmdvme -ww 0x1005101a 0x1

# ../cmdvme/cmdvme -ww 0x40001012 0x3
# ../cmdvme/cmdvme -ww 0x40011012 0x3

# ../cmdvme/cmdvme -ww 0x20006020 0x82
# ../cmdvme/cmdvme -ww 0x20016020 0x82
# ../cmdvme/cmdvme -ww 0x20026020 0x82
# ../cmdvme/cmdvme -ww 0x20036020 0x82
# ../cmdvme/cmdvme -ww 0x20046020 0x82
# ../cmdvme/cmdvme -ww 0x20056020 0x82
# ../cmdvme/cmdvme -ww 0x20006020 0x8A


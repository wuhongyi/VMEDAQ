#!/bin/sh


# CBLT/MCST ADDR For v830AC
#Default setting (i.e. at power ON or after hardware reset) is 0xAA
../cmdvme/cmdvme -ww 0x5000111c 0xbb


#for ADCS
# CBLT/MCST ADDR
# Default (at power on ) 0xaa
../cmdvme/cmdvme -ww 0x10001004 0xbb
../cmdvme/cmdvme -ww 0x10011004 0xbb
../cmdvme/cmdvme -ww 0x10021004 0xbb
# ../cmdvme/cmdvme -ww 0x10031004 0xbb
# ../cmdvme/cmdvme -ww 0x10041004 0xbb
# ../cmdvme/cmdvme -ww 0x10051004 0xbb
# ../cmdvme/cmdvme -ww 0x10061004 0xbb
# ../cmdvme/cmdvme -ww 0x10071004 0xbb

# CBLT/MCST ADDR For v1190AC/v1290A
../cmdvme/cmdvme -ww 0x40001010 0xbb
# ../cmdvme/cmdvme -ww 0x40011010 0xbb


# CBLT ADDR For MADC32
# CBLT ADDR 0xAA (default)
../cmdvme/cmdvme -ww 0x20006022 0xbb
../cmdvme/cmdvme -ww 0x20016022 0xbb
# ../cmdvme/cmdvme -ww 0x20026022 0xbb
# ../cmdvme/cmdvme -ww 0x20036022 0xbb
# ../cmdvme/cmdvme -ww 0x20046022 0xbb
# ../cmdvme/cmdvme -ww 0x20056022 0xbb
# ../cmdvme/cmdvme -ww 0x20066022 0xbb
# ../cmdvme/cmdvme -ww 0x20076022 0xbb


# MCST ADDR For MADC32
# MCST ADDR 0xbb (default)
../cmdvme/cmdvme -ww 0x20006024 0xdd
../cmdvme/cmdvme -ww 0x20016024 0xdd
# ../cmdvme/cmdvme -ww 0x20026024 0xdd
# ../cmdvme/cmdvme -ww 0x20036024 0xdd
# ../cmdvme/cmdvme -ww 0x20046024 0xdd
# ../cmdvme/cmdvme -ww 0x20056024 0xdd
# ../cmdvme/cmdvme -ww 0x20066024 0xdd
# ../cmdvme/cmdvme -ww 0x20076024 0xdd


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

../cmdvme/cmdvme -ww 0x5000111e 0x2

../cmdvme/cmdvme -ww 0x1000101a 0x3
../cmdvme/cmdvme -ww 0x1001101a 0x3
../cmdvme/cmdvme -ww 0x1002101a 0x3
# ../cmdvme/cmdvme -ww 0x1003101a 0x3
# ../cmdvme/cmdvme -ww 0x1004101a 0x3
# ../cmdvme/cmdvme -ww 0x1005101a 0x3
# ../cmdvme/cmdvme -ww 0x1006101a 0x3
# ../cmdvme/cmdvme -ww 0x1007101a 0x3

../cmdvme/cmdvme -ww 0x40001012 0x3
# ../cmdvme/cmdvme -ww 0x40011012 0x3

../cmdvme/cmdvme -ww 0x20006020 0x82
../cmdvme/cmdvme -ww 0x20016020 0x8A
# ../cmdvme/cmdvme -ww 0x20026020 0x82
# ../cmdvme/cmdvme -ww 0x20036020 0x82
# ../cmdvme/cmdvme -ww 0x20046020 0x82
# ../cmdvme/cmdvme -ww 0x20056020 0x82
# ../cmdvme/cmdvme -ww 0x20006020 0x8A






# MADC32   CBLT MCST control register 0x6020 default 0x0
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

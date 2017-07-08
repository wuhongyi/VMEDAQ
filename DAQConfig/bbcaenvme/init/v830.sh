#!/bin/sh
cmd="../cmdvme/cmdvme"
BASEADDR=0x5000

echo "V830 /"
# Scaler GEO address 
${cmd} -ww ${BASEADDR}1110 30
##GEO Address Register
#0x1110, D16, read only; write cycles allowed for version without PAUX connector) 
# V830 AA (PAUX): This register contains the geographical address of the module, i.e. the slot number picked up from the JAUX connector on the VME backplane. 
# GEO [4...0] corresponds to A23...A19 address lines, with AM set to 0x2F: each slot has a relevant number whose binary encoding consists of the GEO ADDR 4 to 0. If a write access to the GEO register is performed in the versions with the PAUX connector, the,module does not respond and the bus will go in timeout. 
# V830 AC (NO PAUX): In the versions without the PAUX connector this register can be also written. In this case, the bits of the GEO Address register are set to 1 by default. In CBLT operation it is up to the User to write the correct GEO address of the module in this register before operating so that the GEO address will be contained in the HEADER words for data identification. 
# N.B.: In the case of versions without the PAUX connector, addressing via geographical address is not available. Although in these versions it is possible to perform a write access to the GEO Address Register for data identification during CBLT operation (see § 4.6.6), avoid to use the GEO Address Register for addressing purposes.
# N.B.: a write access to the GEO Address register causes a CLEAR of the module. 


# Software clear
# 0x1122, MCST, D16, write only /  Software Clear Register
${cmd} -ww ${BASEADDR}1122 0x1 #A write access to this register allows performing a reset of the Counters Registers, the MEB, the Trigger Counter Register and the MEB Event Number Register. 


#0x110C, MCST, D16, write only /  Control Bit Clear Register
${cmd} -ww ${BASEADDR}110c 0x11  #???这里是否有问题？ 0x3
#0x110A, MCST, D16, write only / Control Bit Set Register 
${cmd} -ww ${BASEADDR}110a 0x01  #Trigger random (external or from VME)

#/usr/nbbq/bin/cmdvme -ww 0x50001108 0x0
##/usr/nbbq/bin/cmdvme -ww 0x50001108 0x11

#Set enable channel  这里设置开启路数
${cmd} -lw ${BASEADDR}1100 0x000000ff


#0x110A, MCST, D16, write only / Control Bit Set Register 
${cmd} -ww ${BASEADDR}110a 0x30 #  the module generates a Bus error to finish a block transfer or to warn that the MEB is empty, when accessed.  /  Header writing enabled.

#在 cblt.sh 文件中设置
#Default setting (i.e. at power ON or after hardware reset) is 0xAA
#${cmd} -ww ${BASEADDR}111c 0xbb


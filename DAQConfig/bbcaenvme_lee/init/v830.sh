echo "V830 /"
# Scaler GEO address 
../cmdvme/cmdvme -ww 0x50001110 30
# Software clear 
../cmdvme/cmdvme -ww 0x50001122 0x1
# Bit clear register 
##/usr/nbbq/bin/cmdvme -ww 0x5000110c 0x4
# Set control register
../cmdvme/cmdvme -ww 0x5000110c 0x11
../cmdvme/cmdvme -ww 0x5000110a 0x01
#/usr/nbbq/bin/cmdvme -ww 0x50001108 0x0
#Set enable channel  这里设置开启路数
../cmdvme/cmdvme -lw 0x50001100 0x000000ff
# Set CBLT Addr
##/usr/nbbq/bin/cmdvme -ww 0x50001108 0x11
../cmdvme/cmdvme -ww 0x5000110a 0x30
#../cmdvme/cmdvme -ww 0x5000111c 0xaa


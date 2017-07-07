echo "V830 /"
# Software clear 
/usr/nbbq/bin/cmdvme -ww 0x50001122 0x1
# Scaler GEO address 
/usr/nbbq/bin/cmdvme -ww 0x50001110 0x0
# Bit clear register 
##/usr/nbbq/bin/cmdvme -ww 0x5000110c 0x4
# Set control register
/usr/nbbq/bin/cmdvme -ww 0x5000110a 0x11
#/usr/nbbq/bin/cmdvme -ww 0x50001108 0x0
#Set enable channel
/usr/nbbq/bin/cmdvme -lw 0x50001100 0xfff
# Set CBLT Addr
##/usr/nbbq/bin/cmdvme -ww 0x50001108 0x11
##/usr/nbbq/bin/cmdvme -ww 0x50001108 0x11
#/usr/nbbq/bin/cmdvme -ww 0x5000111c 0xee


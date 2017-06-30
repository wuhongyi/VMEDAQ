#!/bin/sh
echo "v977"
/usr/nbbq/bin/cmdvme -am 0x9
/usr/nbbq/bin/cmdvme -ww 0x10 0x01 #output clear register
/usr/nbbq/bin/cmdvme -ww 0x20 0x0 #interrupt level=0
/usr/nbbq/bin/cmdvme -ww 0xa 0x0  #default level
/usr/nbbq/bin/cmdvme -ww 0x0 0x0 #all output set register default value
/usr/nbbq/bin/cmdvme -ww 0x2 0x0 #all channels inputs are enabled default value
/usr/nbbq/bin/cmdvme -ww 0xc 0x0 #all channels outputs are enabled default value
/usr/nbbq/bin/cmdvme -ww 0xe 0x0 #all channels interrupt are unmasked default value


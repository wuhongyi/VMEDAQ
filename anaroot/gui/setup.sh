#!/bin/bash

. /home/psp14/exp/root/v5.34.14/bin/thisroot.sh

export MYQTDIR=/home/psp14/exp/qt/install
export MYQWTDIR=/home/psp14/exp/qt/qwt-5.2
export LD_LIBRARY_PATH=$MYQTDIR/lib:$MYQWTDIR/lib:$TARTSYS/lib:$LD_LIBRARY_PATH
export PATH=$MYQTDIR/bin:$PATH
export LD_LIBRARY_PATH=$HOME/exp/minos/install/pentium-linux-release/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$HOME/exp/minos/liboffline:$LD_LIBRARY_PATH

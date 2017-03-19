#! /bin/bash -f

#EXP_DIR=$HOME/exp/sm_com/anaroot
#EXP_DIR=$HOME/exp/exp1205_dayone/anaroot
EXP_DIR=$HOME/exp
ROOT_DIR=/usr/local/
#ROOT_DIR=/cern/root/

source $ROOT_DIR/bin/thisroot.sh

if [ -z $1 ]; then
    echo ' '
    echo ' == ANAROOT_SETUP =='
    echo ' '
    echo -n ' Please Input Your Name: '
    read  ANAROOT_USER
    if [ -z $ANAROOT_USER ]; then
        echo ' exit login script'
        echo
        return;
    fi
    echo ' Name : ' $ANAROOT_USER
else
    ANAROOT_USER=$1
fi

ANAROOT_WORK=$EXP_DIR/users/$ANAROOT_USER

if [ ! -d $ANAROOT_WORK ]; then
    echo ' '
    echo -n ' New user! Do you create new directory for this user? [y/n]: '
    while :
    do
	read ans
	case $ans in
	    [yY])
		cp -r $EXP_DIR/users/default $ANAROOT_WORK/;
		break;;
	    [nN])
		echo ' ';
		return;
	esac
	echo -n " y/n: "
    done
else
    echo ' '
    echo ' Your already have work directory.'
fi

export TARTSYS=$ANAROOT_WORK/src
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TARTSYS/lib

#For MINOS
export LD_LIBRARY_PATH=$HOME/exp/minos/install/pentium-linux-release/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$HOME/exp/minos/liboffline:$LD_LIBRARY_PATH
export PKG_CONFIG_PATH=$HOME/exp/minos/install/pentium-linux-release/lib/pkgconfig
export XMLDIPOLESH=$HOME/epics/xmldipole.sh

alias acd='cd '$ANAROOT_WORK

echo ""
echo " Set TARTSYS = $TARTSYS"
echo " Set acd = cd $ANAROOT_WORK"

echo ' '
echo ' Start ANAROOT'
echo ' '

# echo ' '
# echo ' ================================================================== '
# echo ' ======================== ENJOY ANAROOT ! ========================= '
# echo ' ================================================================== '
# echo ' '

unset EXP_DIR
unset ROOT_DIR
unset ANAROOT_WORK
unset ANAROOT_USER

acd

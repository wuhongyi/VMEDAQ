#!/bin/bash

if [ -z $1 ]
then
echo "Usage: getAnaLoop.sh [example analoop]"
echo "Example: getAnaLoop.sh TAlRawDataExample"
echo "select example which you want to modify:"
ls $TARTSYS/sources/AnaLoop/Core/src/ | grep TAl | grep Example | grep -v TAlEnc | grep -v "~" | sed s/\.cc//g | sed s/\.o//g | sort | uniq
ls $TARTSYS/sources/AnaLoop/Example/src/ | grep TAl | grep Example | grep -v TAlEnc | grep -v "~" | sed s/\.cc//g | sed s/\.o//g | sort | uniq
exit 0
fi

outcc=`echo $1 | sed s/Example//g`.cc
outhh=`echo $1 | sed s/Example//g`.hh

if [ -f $TARTSYS/sources/AnaLoop/Core/src/$1.cc ]
then
 echo "cp $1.cc $outcc"
 cp $TARTSYS/sources/AnaLoop/Core/src/$1.cc $outcc
 echo "cp $1.hh $outhh"
 cp $TARTSYS/sources/AnaLoop/Core/include/$1.hh $outhh
elif [ -f $TARTSYS/sources/AnaLoop/Example/src/$1.cc ]
then
 echo "cp $1.cc $outcc"
 cp $TARTSYS/sources/AnaLoop/Example/src/$1.cc $outcc
 echo "cp $1.hh $outhh"
 cp $TARTSYS/sources/AnaLoop/Example/include/$1.hh $outhh
fi

if [ -f $outcc ]
then
 for ff in $outcc $outhh
 do
 sed s/EXAMPLE//g $ff | sed s/Example//g > /tmp/$ff
 mv /tmp/$ff .
 done
fi

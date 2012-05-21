#!/bin/sh
if [ "$1" == "" ] ; then
  echo "usage: $0 a.bin"
  exit -1
fi

FN=./floppy.img
if [ -f $FN ]
then
    echo "$FN exists"
else
  echo "$FN doesn't exist, create $FN"
  dd if=/dev/zero of=floppy.img skip=1 seek=1 bs=512 count=2879
fi

echo ""

echo "write $1 to $FN"
dd if=$1 of=$FN bs=1 count=512 conv=notrunc




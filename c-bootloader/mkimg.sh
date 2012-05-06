#!/bin/sh
if [ "$1" == "" ] ; then
  echo "usage: $0 a.bin"
  exit -1
fi

dd if=$1 of=floppy.img bs=1 count=512 conv=notrunc

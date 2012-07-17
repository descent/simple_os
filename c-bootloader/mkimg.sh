#!/bin/sh

#if [ "$1" == "" ] ; then
#  echo "usage: $0 a.bin"
#  exit -1
#fi

FN=./floppy.img
BIN=""

# ref: http://wiki.bash-hackers.org/howto/getopts_tutorial
while getopts "b:d:" opt; do
  case $opt in
    b)
      echo $OPTARG
      BIN=$OPTARG
      ;;
    d)
      echo $OPTARG
      FN=$OPTARG
      ;;
    \?)
      echo "usage: $0 -b bin -d dev"
      ;;
    *)
      echo "xx invalid"
      ;;
  esac

done

if [ -e $FN ]
then
    echo "$FN exists"
else
  echo "$FN doesn't exist, create $FN"
  dd if=/dev/zero of=floppy.img skip=1 seek=1 bs=512 count=2879
fi

echo ""

echo "write $BIN to $FN"
dd if=$BIN of=$FN bs=1 count=512 conv=notrunc




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
      echo "ex: $0 -b boot.bin -d boot.img"
      echo "ex: $0 -b boot.bin -d /dev/sdz"
      exit 0
      ;;
    *)
      echo "xx invalid"
      exit 0
      ;;
  esac

done

if [ -e $FN ]
then
    echo "$FN exists"
else
  echo "$FN doesn't exist, create $FN"
  dd if=/dev/zero of=$FN skip=1 seek=1 bs=512 count=2879
fi

echo ""


if [ -n "$BIN" ]; then
  echo "write $BIN to $FN"
  dd if=$BIN of=$FN bs=1 count=512 conv=notrunc
fi





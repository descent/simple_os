#!/bin/sh
NUM=`objdump -d -m i8086 cppb.elf|grep __cxa_atexit| wc -l`
sed -i "s/#define DOBJS_SIZE.*/#define DOBJS_SIZE $NUM/" cpp_abi.h 

#!/bin/sh
# test bss program
# make c_init.bin
dd if=c_init.bin of=boot.img bs=512 count=1
dd if=/dev/zero of=boot.img skip=1 seek=1 bs=512 count=2879

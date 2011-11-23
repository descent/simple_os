#!/bin/sh
dd if=/dev/zero of=floppy.img bs=1024 count=1440
dd if=vbr.bin of=floppy.img bs=1 count=512 conv=notrunc


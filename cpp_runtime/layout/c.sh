#!/bin/sh
sudo mount -o loop dos622.img  /floppy/
sudo cp cppb.bin  /floppy/cppb.com
sudo  umount /floppy 


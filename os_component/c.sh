#!/bin/sh
sudo mount -o loop dos622.img  /floppy/
sudo cp bhp.bin  /floppy/bhp.com
sudo  umount /floppy 


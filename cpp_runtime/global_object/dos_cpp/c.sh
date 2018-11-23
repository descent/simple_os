#!/bin/sh
sudo mount -o loop dos622.img  /floppy/
sudo cp -v cppb.bin  /floppy/cppb.com
sudo cp -v nullptr.bin  /floppy/nullptr.com
sudo cp -v reloc.bin  /floppy/reloc.com
sudo  umount /floppy 


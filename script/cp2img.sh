#!/bin/sh
# copy needed files to floppy image.

#make c_init_by_boot_loader.bin
make kloaderp.bin
make p_kernel.elf
sudo  mount -o loop boot.img /mnt/
#sudo  cp kloader.bin  /mnt/loader.bin
sudo  cp -v kernel_loader/kloaderp.bin  /mnt/loader.bin
#sudo  cp kloader.bin  /mnt/test.bin
sudo  cp kernel.bin  /mnt/
sudo  cp kernel.elf  /mnt/
sudo  cp -v p_kernel.elf  /mnt/
sudo  cp rom.fs  /mnt/
sudo  cp /home/descent/git/write_os/src/chapter3/1.5/idt.com /mnt/
sudo  cp /home/descent/git/write_os/src/chapter3/1.5/idt.com.elf /mnt/idt.elf
#sudo  cp c_init_by_boot_loader.bin  /mnt/loader.bin
sudo  ls -l /mnt 
sudo  umount /mnt 

#sudo  mount -o loop dos6.22_data.img /mnt/
#sudo  cp c_init_by_boot_loader.bin  /mnt/c.com
#sudo  umount /mnt 

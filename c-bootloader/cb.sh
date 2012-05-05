gcc -c cb.c 
ld -static -Tl.ld -nostdlib  -o cb.elf cb.o 
objcopy -R .pdr -R .comment -R.note -S -O binary cb.elf cb.bin
dd if=cb.bin of=floppy.img bs=1 count=512 conv=notrunc

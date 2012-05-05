gcc -c -g -Os -march=i686 -ffreestanding -Wall -Werror -I. -o vbr.o vbr.c
ld -static -Tlinker.ld -nostdlib --nmagic -o vbr.elf vbr.o
objcopy -O binary vbr.elf vbr.bin

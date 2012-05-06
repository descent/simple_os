#gcc -c -g -Os -march=i686 -ffreestanding -Wall -Werror -I. -o vbr.o vbr.c
#ld -static -Tlinker.ld -nostdlib --nmagic -o vbr.elf vbr.o
#objcopy -O binary vbr.elf vbr.bin
CFLAGS = -fno-stack-protector -std=c99 -Os -march=i686 -ffreestanding -Wall
CB_CFLAGS = -fno-stack-protector -std=c99 -march=i686 -ffreestanding -Wall

vbr.bin: vbr.elf
	objcopy -O binary $< $@

vbr.elf: vbr.o
	ld -static -Tlinker.ld -nostdlib --nmagic -o $@ $<

vbr.o: vbr.c code16gcc.h
	#gcc -g -Os -march=i686 -ffreestanding -Wall -Werror -I. -c $<
	gcc -Os -march=i686 -ffreestanding -Wall -Werror -I. -c $<

vbr-lba.bin: vbr-lba.elf
	objcopy -O binary $< $@

vbr-lba.elf: vbr-lba.o
	ld -static -Tlinker.ld -nostdlib --nmagic -o $@ $<

vbr-lba.o: vbr-lba.c code16gcc.h
	#gcc -g -Os -march=i686 -ffreestanding -Wall -Werror -I. -c $<
	gcc -Os -march=i686 -ffreestanding -Wall -Werror -I. -c $<

#fat: fat.o
#	gcc -std=c99 -Os -march=i686 -ffreestanding -Wall -Werror -I. -o $@ $<
	#gcc -std=c99 -o $@ $<

fat.bin: fat.bin.elf
	objcopy -R .pdr -R .comment -R.note -S -O binary $< $@
fat.bin.elf: fat.bin.o
	$(LD) -o $@ -Tlinker.ld $^
fat.com: fat.com.elf
	objcopy -R .pdr -R .comment -R.note -S -O binary $< $@
fat.com.elf: fat.com.o
	$(LD) -o $@ -Tlinker_dos.ld $^
fat.bin.o: fat.c
	gcc $(CFLAGS) -I. -o $@ -c $<
fat.com.o: fat.c
	#gcc $(CFLAGS) -I. -o $@ -c $<
	gcc -DDOS_COM $(CFLAGS) -I. -o $@ -c $<
	#gcc -std=c99 -Os -march=i686 -ffreestanding -Wall -Werror -I. -c $<
	#gcc -std=c99 -c $<
fat.s: fat.c
	gcc $(CFLAGS) -I. -S $<

cb.s: cb.c
	gcc $(CB_CFLAGS) -I. -S $<
cb.o: cb.c
	gcc $(CB_CFLAGS) -c $<
cb.elf: cb.o
	ld -static -Tl.ld -nostdlib -M -o cb.elf cb.o > cb.elf.map
cb.bin: cb.elf
	objcopy -R .pdr -R .comment -R.note -S -O binary cb.elf cb.bin

bh.s.o: bh.s
	as -o $@ $<
bh.s.elf: bh.s.o
	ld -Tas.ld -o $@ $<

bh.s.bin: bh.s.elf
	objcopy -O binary $< $@

bh.asm.bin: bh.asm
	nasm bh.asm -o bh.asm.bin

clean:
	rm -rf *.o

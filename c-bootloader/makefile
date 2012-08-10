#gcc -c -g -Os -march=i686 -ffreestanding -Wall -Werror -I. -o vbr.o vbr.c
#ld -static -Tlinker.ld -nostdlib --nmagic -o vbr.elf vbr.o
#objcopy -O binary vbr.elf vbr.bin
CFLAGS = -fno-stack-protector -std=c99 -Os -march=i686 -ffreestanding -Wall
#CB_CFLAGS = -fno-stack-protector -std=c99 -march=i686 -ffreestanding -Wall
CB_CFLAGS = -fno-stack-protector -std=c99 -m32 -ffreestanding -Wall -g
CB_LDFLAGS = -m elf_i386

CPPFLAGS = -m32  -g -Wall -Wextra -Werror -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-rtti -fno-stack-protector


all: bh.asm.bin bh.s.bin  cb.bin bhp.bin vbr.bin

vbr.bin: vbr.elf
	objcopy -O binary $< $@

vbr.elf: vbr.o
	ld -m elf_i386 -static -Tlinker.ld -nostdlib --nmagic -o $@ $<

vbr.o: vbr.c code16gcc.h
	#gcc -g -Os -march=i686 -ffreestanding -Wall -Werror -I. -c $<
	#gcc -Os -march=i686 -ffreestanding -Wall -Werror -I. -c $<
	gcc -Os $(CB_CFLAGS) -I. -c $<

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
cb.elf: cb.o l.ld
	ld $(CB_LDFLAGS) -static -Tl.ld -nostdlib -M -o cb.elf cb.o > cb.elf.map
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
bhp.o: bhp.S
	gcc -c bhp.S
bhp.elf: bhp.o
	ld -Tas.ld -o bhp.elf bhp.o
bhp.bin: bhp.elf
	objcopy -O binary $< $@

cppb.bin: cppb.elf
	objcopy -R .pdr -R .comment -R.note -S -O binary $< $@
cppb.elf: cppb.o l.ld
	ld $(CB_LDFLAGS) -static -Tl.ld -nostdlib -M -o $@ $< > cb.elf.map
cppb.o: cppb.cpp
	g++ $(CPPFLAGS) -c $<


clean:
	rm -rf *.o

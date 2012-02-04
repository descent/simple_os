#gcc -c -g -Os -march=i686 -ffreestanding -Wall -Werror -I. -o vbr.o vbr.c
#ld -static -Tlinker.ld -nostdlib --nmagic -o vbr.elf vbr.o
#objcopy -O binary vbr.elf vbr.bin
CFLAGS = -fno-stack-protector -std=c99 -Os -march=i686 -ffreestanding -Wall

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

fat.com: fat.elf
	objcopy -R .pdr -R .comment -R.note -S -O binary $< $@
fat.elf: fat.o
	$(LD) -o $@ -Tlinker_dos.ld $^
fat.o: fat.c
	gcc $(CFLAGS) -I. -c $<
	#gcc -std=c99 -Os -march=i686 -ffreestanding -Wall -Werror -I. -c $<
	#gcc -std=c99 -c $<
fat.s: fat.c
	gcc $(CFLAGS) -I. -S $<
clean:
	rm -rf *.o

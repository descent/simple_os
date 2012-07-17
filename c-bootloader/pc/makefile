CB_CFLAGS = -fno-stack-protector -std=c99 -march=i686 -ffreestanding -Wall -m32
CB_LDFLAGS = -m elf_i386

bhp.bin: bhp.elf
	objcopy -O binary $< $@
cb.s: cb.c
	gcc $(CB_CFLAGS) -I. -S $<
cb.o: cb.c
	gcc $(CB_CFLAGS) -c $<

bhp.o: bhp.S
	gcc -m32 -c bhp.S

bhp.elf: bhp.o cb.o
	ld -m elf_i386 -Tl.ld -o bhp.elf $^

clean:
	rm -rf *.o *.bin *.elf

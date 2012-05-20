CB_CFLAGS = -fno-stack-protector -std=c99 -march=i686 -ffreestanding -Wall

cb.s: cb.c
	gcc $(CB_CFLAGS) -I. -S $<
cb.o: cb.c
	gcc $(CB_CFLAGS) -c $<

bhp.o: bhp.S
	gcc -c bhp.S

bhp.elf: bhp.o cb.o
	ld -Tl.ld -o bhp.elf $^
bhp.bin: bhp.elf
	objcopy -O binary $< $@

clean:
	rm -rf *.o *.bin *.elf

#CFLAGS = -fno-stack-protector -std=c99 -march=i686 -ffreestanding -Wall -g
CFLAGS = -fno-stack-protector -ffreestanding -g

boot.img: c_init.bin
	dd if=$< of=$@ bs=512 count=1
	dd if=/dev/zero of=$@ skip=1 seek=1 bs=512 count=2879

c_init.bin: c_init.elf
	objcopy -R .pdr -R .comment -R.note -S -O binary $< $@

c_init.elf: c_init.o b.o bss.lds
	ld -nostdlib -g -o $@ -Tbss.lds c_init.o b.o 
c_init.o: c_init.s
	as -o $@ $<
b.o: b.c
	gcc $(CFLAGS) -c $<
b.s: b.c
	gcc -S $(CFLAGS) -c $<

.PHONE: clean

clean:
	rm -rf *.o *.elf *.bin *.img

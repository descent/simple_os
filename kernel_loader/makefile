CFLAGS = -std=c99 -fno-stack-protector -m32 -ffreestanding -fno-builtin -g -O0
ASFLAGS = --32
LDFLAGS = -m elf_i386

# enter protected mode kernel loader
kloaderp.bin: kloaderp.bin.elf
	objcopy -R .pdr -R .comment -R.note -S -O binary $< $@

kloaderp.bin.elf: kloader_init.o kernel_loader.o protected_code.o
	ld $(LDFLAGS) -nostdlib -g -o $@ -Tklp.ld $^

kloader_init.o: kloader_init.S
	gcc $(CFLAGS) -o $@ -c $<

protected_code.o: protected_code.c
	gcc $(CFLAGS) -c $<
protected_code.s: protected_code.c
	gcc $(CFLAGS) -o $@ -S $<
#kernel_loader.o: kernel_loader.c elf.h
#	gcc -std=c99 $(CFLAGS) -c $<
kernel_loader.o: kernel_loader.s
	as $(ASFLAGS) -o $@ $<
#kernel_loader.s: kernel_loader.c elf.h
kernel_loader.s: kernel_loader.c 
	gcc -std=c99 $(CFLAGS) -o $@ -S $<

.PHONE: clean 

clean:
	rm -rf *.o *.elf *.bin kernel_loader.s

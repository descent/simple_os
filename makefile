#CFLAGS = -fno-stack-protector -std=c99 -march=i686 -ffreestanding -Wall -g
#CFLAGS = -fno-stack-protector -ffreestanding -fno-builtin -g
CFLAGS = -std=c99 -fno-stack-protector -m32 -ffreestanding -fno-builtin -g -Iinclude
ASFLAGS = --32
LDFLAGS = -m elf_i386


boot1.img: c_init.bin
	dd if=$< of=$@ bs=512 count=1
	dd if=/dev/zero of=$@ skip=1 seek=1 bs=512 count=2879

c_init.bin: c_init.elf
	objcopy -R .pdr -R .comment -R.note -S -O binary $< $@

c_init.elf: c_init.o b.o bss.lds
	ld $(LDFLAGS) -nostdlib -g -o $@ -Tbss.lds c_init.o b.o 
c_init.o: c_init.s
	as $(ASFLAGS) -o $@ $<
b.o: b.c
	gcc $(CFLAGS) -c $<
b.s: b.c
	gcc -S $(CFLAGS) -c $<

c_init_by_boot_loader.bin: c_init_by_boot_loader.elf
	objcopy -R .pdr -R .comment -R.note -S -O binary $< $@

c_init_by_boot_loader.elf: c_init_by_boot_loader.o b_by_bootloader.o switch_vga_mode.o bss_dos.lds
	ld $(LDFLAGS) -nostdlib -g -o $@ -Tbss_dos.lds c_init_by_boot_loader.o b_by_bootloader.o switch_vga_mode.o
c_init_by_boot_loader.o: c_init_by_boot_loader.s
	as $(ASFLAGS) -o $@ $<
b_by_bootloader.o: b_by_bootloader.c
	gcc $(CFLAGS) -c $<
b_by_bootloader.s: b_by_bootloader.c
	gcc -S $(CFLAGS) $<
switch_vga_mode.o: switch_vga_mode.c
	gcc $(CFLAGS) -c $<
switch_vga_mode.s: switch_vga_mode.c
	gcc -S $(CFLAGS) $<

kloader.bin: kernel_loader.elf
	objcopy -R .pdr -R .comment -R.note -S -O binary $< $@

kernel_loader.elf: c_init_by_boot_loader.o kernel_loader.o 
	ld $(LDFLAGS) -nostdlib -g -o $@ -Tbss_dos.lds $^

kernel.bin: kernel.elf
	objcopy -R .pdr -R .comment -R.note -S -O binary $< $@
kernel.elf: kernel.o
	ld $(LDFLAGS) -nostdlib -g -o $@ -Tbss_dos.lds $^
kernel.o: kernel.s
	as $(ASFLAGS) -o $@ $<


p_kernel.elf: p_kernel.o start.o process.o clock.o asm_func.o syscall.o asm_syscall.o storage.o romfs.o
	ld $(LDFLAGS) -nostdlib -M -g -o $@ -Tk.ld $^ > $@.map

p_kernel.o: p_kernel.s
	as --32 -o $@ $<

start.o: start.c type.h protect.h process.h
	gcc $(CFLAGS) -c $<

start.s: start.c type.h protect.h process.h
	gcc $(CFLAGS) -o $@ -S $<

process.o: process.c process.h type.h protect.h
	gcc $(CFLAGS) -c $<

clock.o: clock.c clock.h type.h process.h protect.h
	gcc $(CFLAGS) -c $<

storage.o: storage.c storage.h type.h
	gcc $(CFLAGS) -c $<

asm_func.o: asm_func.s
	as --32 -o $@ $<

asm_func.s: asm_func.S
	gcc $(CFLAGS) -o $@ -E $<

asm_syscall.o: asm_syscall.s
	as --32 -o $@ $<
asm_syscall.s: asm_syscall.S syscall.h process_const.h
	gcc $(CFLAGS) -o $@ -E $<

syscall.o: syscall.c syscall.h
	gcc $(CFLAGS) -c $<

romfs.o: romfs.c type.h include/endian.h
	gcc $(CFLAGS) -c $<


.PHONE: clean distclean

clean:
	rm -rf *.o *.elf *.bin 
distclean:
	rm -rf *.img

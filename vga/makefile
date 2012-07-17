#CFLAGS = -std=c99 -fno-stack-protector -m32 -ffreestanding -fno-builtin -g -O0
#ASFLAGS = --32
#LDFLAGS = -m elf_i386

ifdef COMSPEC
# in windows
include ..\rule.mk
else
include ../rule.mk
endif

set_mode_p.o: set_mode_p.S
	gcc $(CFLAGS) -o $@ -c $<
draw_func.o: draw_func.c draw_func.h ../include/type.h
	gcc $(CFLAGS) -o $@ -c $<


set_mode.bin: set_mode.elf
	objcopy -O binary $< $@
set_mode.o: set_mode.S
	gcc -o $@ -c $<

set_mode.elf: set_mode.o
	ld -Tas.ld -o $@ $<
        


CFLAGS = -DIPC -std=c99 -fno-stack-protector -m32 -ffreestanding -fno-builtin -g -Iinclude -I../include
ASFLAGS = --32
LDFLAGS = -m elf_i386
CC=gcc

%.d: %.c
	set -e; rm -f $@ ; gcc -MM $(CFLAGS) $< > $@.$$$$ ; sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@ ; rm -f $@.$$$$


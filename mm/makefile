#CFLAGS = -std=c99 -fno-stack-protector -m32 -ffreestanding -fno-builtin -g -O0
#ASFLAGS = --32
#LDFLAGS = -m elf_i386

ifdef COMSPEC
# in windows
include ..\rule.mk
else
include ../rule.mk
endif

sources = mm.c
include $(sources:.c=.d)



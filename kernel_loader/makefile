#CFLAGS = -std=c99 -fno-stack-protector -m32 -ffreestanding -fno-builtin -g -O0
#CFLAGS = -std=c99 -fno-stack-protector -m32 -ffreestanding -fno-builtin -g -I../include
#ASFLAGS = --32 -g
#LDFLAGS = -m elf_i386

ifdef COMSPEC
# in windows
include ..\rule.mk
else
include ../rule.mk
endif

# enter protected mode kernel loader
kloaderp.bin: kloaderp.bin.elf
	objcopy -R .pdr -R .comment -R.note -S -O binary $< $@

kloaderp.bin.elf: kloader_init.o kernel_loader.o protected_code.o ../io/k_stdio.o
	ld $(LDFLAGS) -nostdlib -g -o $@ -Tklp.ld $^

kloader_init.o: kloader_init.S
	gcc $(CFLAGS) -o $@ -c $<

protected_code.o: protected_code.c
	gcc $(CFLAGS) -c $<
protected_code.s: protected_code.c
	gcc $(CFLAGS) -o $@ -S $<
#kernel_loader.o: kernel_loader.c elf.h
#	gcc -std=c99 $(CFLAGS) -c $<
#kernel_loader.o: kernel_loader.s
#	as $(ASFLAGS) -o $@ $<
#kernel_loader.s: kernel_loader.c elf.h
#kernel_loader.s: kernel_loader.c ../include/type.h ../include/elf.h ../include/k_string.h ../include/type.h
#	gcc $(CFLAGS) -o $@ -S $<
protected_code.e: protected_code.c
	gcc $(CFLAGS) -E -o $@ -S $<
#kernel_loader.e: kernel_loader.c 
#	gcc $(CFLAGS) -E -o $@ -S $<

../io/k_stdio.o: ../io/k_stdio.c
	(cd ../io; make)



.PHONE: clean 

clean:
	rm -rf *.o *.elf *.bin kernel_loader.s *.d *.e

sources = kernel_loader.c  protected_code.c
include $(sources:.c=.d)


# ref: http://www.gnu.org/software/make/manual/make.html
#%.d: %.c
#@set -e; rm -f $@; \
#$(CC) -M $(CPPFLAGS) $< > $@.$$$$; \
#sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
#rm -f $@.$$$$


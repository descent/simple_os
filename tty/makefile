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

#protected_code.e: protected_code.c
#	gcc $(CFLAGS) -E -o $@ -S $<



%.d: %.c
	set -e; rm -f $@ ; gcc -MM $(CFLAGS) $< > $@.$$$$ ; sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@ ; rm -f $@.$$$$

sources = keyboard.c
include $(sources:.c=.d)


# ref: http://www.gnu.org/software/make/manual/make.html
#%.d: %.c
#@set -e; rm -f $@; \
#$(CC) -M $(CPPFLAGS) $< > $@.$$$$; \
#sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
#rm -f $@.$$$$

.PHONE: clean 

clean:
	rm -rf *.o *.elf *.d *.e

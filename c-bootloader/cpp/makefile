CPPFLAGS = -m32  -g -Wall -Wextra -Werror -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-rtti -fno-stack-protector
LDFLAGS = -m elf_i386

%.dpp: %.cpp
	set -e; rm -f $@ ; gcc -MM $(CFLAGS) $< > $@.$$$$ ; sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@ ; rm -f $@.$$$$

include $(sources:.cpp=.dpp)

CXX=g++

cppb.bin: cppb.elf
	objcopy -R .pdr -R .comment -R.note -S -O binary $< $@
cppb.elf: cppb.o io.o
	ld $(LDFLAGS) -static -Tcpp.ld -nostdlib -M -o $@ $^ > cb.elf.map
clean:
	rm -rf *.o

#CPPFLAGS = -m32  -g -Wall -Wextra -Werror -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-rtti -fno-stack-protector
CPPFLAGS = -m32  -g -Wall -Wextra -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-rtti -fno-stack-protector
LDFLAGS = -m elf_i386

cppb.bin: cppb.elf
	objcopy -R .pdr -R .comment -R.note -S -O binary $< $@

%.dpp: %.cpp
	set -e; rm -f $@ ; g++ -MM $(CFLAGS) $< > $@.$$$$ ; sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@ ; rm -f $@.$$$$

sources = cppb.cpp  io.cpp
include $(sources:.cpp=.dpp)

CXX=g++

cppb.elf: cppb.o io.o
	ld $(LDFLAGS) -static -Tcpp.ld -nostdlib -M -o $@ $^ > cb.elf.map
clean:
	rm -rf *.o *.bin *.elf

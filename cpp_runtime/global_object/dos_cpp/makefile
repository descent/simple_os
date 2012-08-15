#CPPFLAGS = -m32  -g -Wall -Wextra -Werror -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-rtti -fno-stack-protector
CPPFLAGS = -static -m32  -g -Wall -Wextra -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-rtti -fno-stack-protector
LDFLAGS = -m elf_i386 -static

cppb.bin: cppb.elf
	objcopy -R .pdr -R .comment -R.note -S -O binary $< $@
cppb.elf: cpp_init.o cppb.o io.o cpp_abi.o
	ld $(LDFLAGS) -Tcpp.ld -nostdlib -M -o $@ $^ > $@.map
cpp_init.o: cpp_init.s
	$(CXX) $(CPPFLAGS) -c $<

cpp_init.s: cpp_init.S
	$(CXX) -CC $(CPPFLAGS) -E $< > $@

%.dpp: %.cpp
	set -e; rm -f $@ ; g++ -MM $(CFLAGS) $< > $@.$$$$ ; sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@ ; rm -f $@.$$$$

sources = cppb.cpp  io.cpp cpp_abi.cpp
include $(sources:.cpp=.dpp)

CXX=g++

clean:
	rm -rf *.o *.bin *.elf

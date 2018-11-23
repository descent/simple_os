#CPPFLAGS = -m32  -g -Wall -Wextra -Werror -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-rtti -fno-stack-protector
CPPFLAGS = -static -m32  -g -Wall -Wextra -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-rtti -fno-stack-protector -std=c++17
LDFLAGS = -m elf_i386 -static
CXX=g++

%.bin: %.elf
	objcopy -R .pdr -R .comment -R.note -S -O binary $< $@

new_test.elf: cpp_init.o new_test.o io.o cpp_abi.o obj.o dos_io.o
	ld $(LDFLAGS) -Tcpp.ld -nostdlib -M -o $@ $^ > $@.map

reloc.elf: cpp_init.reloc.o reloc.o dos_io.o
	ld -pie $(LDFLAGS) -Treloc.ld -nostdlib -o $@ $^ 

nullptr.bin: nullptr.elf
	objcopy -R .pdr -R .comment -R.note -S -O binary $< $@
nullptr.elf: cpp_init.o nullptr.o io.o cpp_abi.o obj.o dos_io.o
	ld $(LDFLAGS) -Tcpp.ld -nostdlib -M -o $@ $^ > $@.map

cppb.bin: cppb.elf
	objcopy -R .pdr -R .comment -R.note -S -O binary $< $@
cppb.elf: cpp_init.o cppb.o io.o cpp_abi.o obj.o dos_io.o
	ld $(LDFLAGS) -Tcpp.ld -nostdlib -M -o $@ $^ > $@.map

reloc.o: reloc.cpp
	$(CXX) $(CPPFLAGS) -c $<

cpp_init.o: cpp_init.S
	$(CXX) $(CPPFLAGS) -c $<

cpp_init.reloc.o: cpp_init.S
	$(CXX) -DRELOC $(CPPFLAGS) -c $< -o $@

#cpp_init.o: cpp_init.s
#	$(CXX) $(CPPFLAGS) -c $<
#cpp_init.s: cpp_init.S
#	$(CXX) -CC $(CPPFLAGS) -E $< > $@

%.dpp: %.cpp
	set -e; rm -f $@ ; g++ -MM $(CFLAGS) $< > $@.$$$$ ; sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@ ; rm -f $@.$$$$

sources = cppb.cpp io.cpp cpp_abi.cpp obj.cpp dos_io.cpp
include $(sources:.cpp=.dpp)
#OBJS = $(sources:.cpp=.o)# can not use this method 
#OBJS = cpp_init.o cppb.o io.o cpp_abi.o obj.o


clean:
	rm -rf *.o *.bin *.elf

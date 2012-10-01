CPPFLAGS = -static -m32  -g -Wall -Wextra -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-rtti -fno-stack-protector -Iinclude -I../include
LDFLAGS = -m elf_i386 -static
CXX=g++


bhp.bin: bhp.elf
	objcopy -O binary $< $@
bhp.o: bhp.S
	$(CXX) -m32 -c bhp.S

bhp.elf: bhp.o cpp_init.o main.o io.o cpp_abi.o tool.o 
	ld -m elf_i386 -Tcpp.ld -o bhp.elf $^


#cppb.bin: cppb.elf
#        objcopy -R .pdr -R .comment -R.note -S -O binary $< $@
#cppb.elf: cpp_init.o cppb.o io.o cpp_abi.o obj.o dos_io.o
#        ld $(LDFLAGS) -Tcpp.ld -nostdlib -M -o $@ $^ > $@.map
cpp_init.o: cpp_init.s
	$(CXX) $(CPPFLAGS) -c $<

cpp_init.s: cpp_init.S
	$(CXX) -CC $(CPPFLAGS) -E $< > $@

%.dpp: %.cpp
	set -e; rm -f $@ ; g++ -MM $(CFLAGS) $< > $@.$$$$ ; sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@ ; rm -f $@.$$$$ 
    
sources = main.cpp cpp_abi.cpp io.cpp tool.cpp
include $(sources:.cpp=.dpp)

clean:
	rm -rf *.o *.bin *.elf

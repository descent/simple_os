#CPPFLAGS = -m32  -g -Wall -Wextra -Werror -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-rtti -fno-stack-protector
#CPPFLAGS = -m32  -g -Wall -Wextra -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-rtti -fno-stack-protector
#LDFLAGS = -m elf_i386
CPPFLAGS = -m32 -g -static
cppb: cppb.o io.o
	$(CXX) -static -o $@ $^

%.dpp: %.cpp
	set -e; rm -f $@ ; g++ -MM $(CFLAGS) $< > $@.$$$$ ; sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@ ; rm -f $@.$$$$

sources = cppb.cpp  io.cpp
include $(sources:.cpp=.dpp)

CXX=g++

clean:
	rm -rf *.o

CFLAGS = -std=c99 -fno-stack-protector -m32 -ffreestanding -fno-builtin -g -I../include

all: vfs.o romfs.o

vfs.o: vfs.c ../include/vfs.h ../include/type.h ../include/storage.h
	gcc $(CFLAGS) -c $<

romfs.o: romfs.c ../include/type.h ../include/endian.h ../include/type.h \
 ../include/romfs.h
	gcc $(CFLAGS) -c $<

clean:
	rm -rf *.o

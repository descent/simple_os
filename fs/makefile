CFLAGS = -std=c99 -fno-stack-protector -m32 -ffreestanding -fno-builtin -g -I../include

vfs.o: vfs.c ../include/vfs.h ../include/type.h ../include/storage.h
	gcc $(CFLAGS) -c $<

clean:
	rm -rf *.o

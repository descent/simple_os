#gcc -c -g -Os -march=i686 -ffreestanding -Wall -Werror -I. -o vbr.o vbr.c
#ld -static -Tlinker.ld -nostdlib --nmagic -o vbr.elf vbr.o
#objcopy -O binary vbr.elf vbr.bin
CFLAGS = -fno-stack-protector -std=c99 -Os -march=i686 -ffreestanding -Wall
#CB_CFLAGS = -fno-stack-protector -std=c99 -march=i686 -ffreestanding -Wall
CB_CFLAGS = -fno-stack-protector -std=c99 -m32 -ffreestanding -Wall -g
CB_LDFLAGS = -m elf_i386

all: func
#all: cb.bin

c_init.o: c_init.S
	gcc $(CB_CFLAGS) -I. -c $<

	#gcc -std=c99 -Os -march=i686 -ffreestanding -Wall -Werror -I. -c $<
	#gcc -std=c99 -c $<
fat.s: fat.c
	gcc $(CFLAGS) -I. -S $<

cb.s: cb.c
	gcc $(CB_CFLAGS) -I. -S $<
cb.o: cb.c
	gcc $(CB_CFLAGS) -c $<
cb.elf: c_init.o cb.o 
	ld $(CB_LDFLAGS) -static -Tl.ld -nostdlib -M -o cb.elf $^ > $@.map
cb.bin: cb.elf
	objcopy -R .pdr -R .comment -R.note -S -O binary cb.elf cb.bin

func: func.o asm_func.o
	gcc -o $@ $^
func.o: func.c
	gcc -c $<
asm_func.o: asm_func.S
	gcc -c $<
clean:
	rm -rf *.o

set_mode.bin: set_mode.elf
	objcopy -O binary $< $@
set_mode.o: set_mode.S
	gcc -o $@ -c $<

set_mode.elf: set_mode.o
	ld -Tas.ld -o $@ $<
        


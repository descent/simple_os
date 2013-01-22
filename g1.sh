file p_kernel.elf.gdb
target remote localhost:1234
#b load_init_boot
#b proc_a
#b romfs_namei
#b romfs_init
#b kernel_main
#b alloc_mem
#b sys_write
#b sys_call
#b init_proc
b do_fork
#b init
#b sys_get_ticks
display proc_table[7].p_flags 
display proc_table[3].p_flags 
#display ready_process->p_name
display ready_process->name


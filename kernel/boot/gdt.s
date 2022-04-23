.global load_gdt
.type load_gdt, @function
load_gdt:
    lgdt (gp)
    /* set the bit to enter protected mode */
    mov %cr0, %eax
    or $0x1, %eax
    mov %eax, %cr0
    /* this jump is needed to reload the code segment from the new GDT */
    jmp $0x08, $reload_code_segment /* 0x08 is offset of kernel code segment row in GDT */
reload_code_segment:
    /* now that code segment is reloaded, load the rest */
    mov $0x10, %ax /* 0x10 is offset of kernel data segment row in GDT */
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ret

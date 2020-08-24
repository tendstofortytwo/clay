/* instruct the bootloader to start exectution at the symbol _start */
ENTRY(_start)

/* describe where in the kernel image all the different sections should be */
SECTIONS {
    /* it is conventional to start putting sections at the 1MiB mark */
    . = 1M;

    /* put the .multiboot section first because it needs to be early in
       the kernel image. then we add the .text section */
    .text BLOCK(4K) : ALIGN(4K) {
        *(.multiboot)
        *(.text)
    }

    /* read-only data */
    .rodata BLOCK(4K) : ALIGN(4K) {
        *(.rodata)
    }

    /* read-write data (initialized) */
    .data BLOCK(4K) : ALIGN(4K) {
        *(.data)
    }

    /* read-write data (uninitialized) and stack */
    .bss BLOCK(4K) : ALIGN(4K) {
        *(COMMON)
        *(.bss)
    }

    /* the compiler may produce more sections, by default
       it'll insert them in a segment with the same name.
       feel free to add stuff here. */
}

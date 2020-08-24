/* constants required for multiboot header */
.set ALIGN,		1<<0				/* align modules on page boundaries */
.set MEMINFO,	1<<1				/* provide memory map */
.set FLAGS,		ALIGN | MEMINFO		/* Multiboot 'flag' field */
.set MAGIC,		0x1BADB002			/* Magic constant lets bootloader find header */
.set CHECKSUM,  -(MAGIC + FLAGS)	/* Checksum verifies that everything is Multiboot-compliant */

/*
Declare program as a Multiboot kernel, to enable
booting from standard loaders like GRUB. These magic
values are documented in the multiboot standard and
I should probably read that at some point.
The bootloader will search for this header in the first
8KB of the kernel file, at a 32-bit boundary.
By putting it in its own section, we can force
the header to be within the first 8KB.
*/

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/*
The kernel needs to provide a stack, so that's what
we do next. We make a small slack by creating a symbol 
at the bottom, then allocating 16KB of space, then creating
a symbol at the top. On x86 the stack grows downward, so
the "bottom" of the staack comes above the "top".
*/
.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

.section .text
.global _start
.type _start, @function
_start:
    /*
    The bootloader drops us into 32-bit protected mode,
    with interrupts and paging disabled. The processor's
    state at this point should be as defined in the multiboot
    standard. The kernel at this point has complete access
    to all parts of the machine.
    */

    /*
    first, setup a stack by pointing %esp to the top of the stack
    This has to be done in assembly since even C depends on having
    a stack
    */
    mov $stack_top, %esp

    /*
    Initialize crucial processor state here, before we enter the
    high-level kernel - stuff like GDT and paging should be
    enabled here. Note that the entire processor isn't available
    at this point - floating point instructions, and instruction set
    extensions are not yet available, and some C++ features like
    global constructors and exceptions require runtime support to
    work as well.
    */


    /*
    Enter the high-level kernel. The ABI requires top of stack to be
    16-byte aligned at the tyime of the call instruction. Since
    the stack is at a 16 byte boundary as we have not pushed anything
    to the stack yet, we should be good.
    */
    call kernel_main


    /*
    If we're done with everything, we put the system in an infinite
    loop. To do that:
    1. Disable interrupts using cli (clear interrupt enable flag in
    eflags).
    2. Halt and wait for interrupts with the hlt instruction. Since
    interrupts are disabled, this will lock up the computer.
    3. Jump to the hlt instruction again if we get a non-maskable
    interrupt.
    */
    cli
l:
    hlt
    jmp l


/*
Set the size of the _start symbol to the current location '.'
minus its start. This is useful when debugging or if we implement
call tracing.
*/
.size _start, . - _start

/* 
these headers are not part of the C
standard library but provided by the
compiler itself. stdbool provides booleans,
stddef provides stuff like size_t and NULL,
and stdint provides intX_t and uintX_t types.
*/
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "vga/include/vga.h"

void kernel_main(void) {
    kprintf("Hello, world!\n");
    
    kprintf("This is ");
    kcprintf(vgaColor(VGA_CYAN, VGA_BLACK), "Clay");
    kprintf(".\n\n");

    kprintf("Testing string and character output, \"%s\" and \'%c\'\n", "lmao", '*');
    kprintf("Testing literal percent sign, %%\n");
    kprintf("Testing binary number, 0b%b\n", 42);
    kprintf("Testing octal number, 0%o\n", 69);
    kprintf("Testing unsigned decimal number, %u\n", 2147483648);
    kprintf("Testing signed decimal number, %d or %i\n", -100, 100);
    kprintf("Testing hex number, 0x%x\n", 0xf00dcafe);
    kprintf("Testing invalid formatter %P\n");
    kprintf("Testing interrupts\n");
    asm("int3");
}

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
    terminalInit();

    terminalPrintf("Hello, world!\n");
    
    terminalPrintf("This is ");
    terminalColorPrintf(vgaEntryColor(VGA_CYAN, VGA_BLACK), "Clay");
    terminalPrintf(".\n\n");

    terminalPrintf("Testing string and character output, \"%s\" and \'%c\'\n", "lmao", '*');
    terminalPrintf("Testing literal percent sign, %%\n");
    terminalPrintf("Testing binary number, 0b%b\n", 42);
    terminalPrintf("Testing octal number, 0%o\n", 69);
    terminalPrintf("Testing unsigned decimal number, %u\n", 2147483648);
    terminalPrintf("Testing signed decimal number, %d or %i\n", -100, 100);
    terminalPrintf("Testing hex number, 0x%x\n", 0xf00dcafe);
    terminalPrintf("Testing invalid formatter %P\n");

    for(;;);
}

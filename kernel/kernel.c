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

    terminalWriteStr("Hello, world!\n");
    terminalWriteStr("This is Clay.\n");

    // write a bunch of lines to test scrolling
    for(int32_t i = 0; i < 25; ++i) {
        terminalPrintf("Testing line %d\n", i);
    }

    terminalPrintf("Testing string output, \"%s\"\n", "lmao");
    terminalPrintf("Testing character output, \'%c\'\n", '*');
    terminalPrintf("Testing literal percent sign, %%\n");
    terminalPrintf("Testing binary number, %b\n", 42);
    terminalPrintf("Testing hex number, %h\n", 0x1a2b3c4d);
    terminalPrintf("Testing invalid formatter %P\n");
}

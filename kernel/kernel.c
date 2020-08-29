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
    for(uint32_t i = 0; i < 25; ++i) {
        terminalWriteStr("Testing line ");
        terminalPutChar('0' + ((i/10)%10) );
        terminalPutChar('0' + (i%10) );
        terminalPutChar('\n');
    }
}

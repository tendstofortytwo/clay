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
}

typedef struct {
    uint32_t base, limit;
    uint8_t access, flags;
} GDTContents;

typedef struct {
    uint16_t limit_lo;
    uint16_t base_lo;
    uint8_t base_mid;
    uint8_t access;
    uint8_t limit_hi_flags; // low 4 bits contain limit_hi, high 4 bits contain flags
    uint8_t base_hi;
} __attribute__((packed)) GDTRow;

typedef struct {
    uint16_t limit;
    void* base; // 32-bit ptr for 32-bit systems, 64-bit ptr for 64-bit systems, should be fine??
} __attribute__((packed)) GDTPtr;

void fill_gdt_row(GDTRow* target, GDTContents* source) {
    if(source->limit > 0xFFFFF) {
        // todo: write a kerror() function for this
        terminalColorPrintf(vgaEntryColor(VGA_RED, VGA_BLACK), "Invalid GDT limit %d\n", source->limit);
        for(;;);
    }

    // the row has to be filled out in a weird way to be compatible with the 286 (?)

    target->limit_lo = source->limit & 0xFFFF;
    target->limit_hi_flags = (source->limit >> 16) & 0x0F; // 0f should be fine? since addr is <=20bits anyway?

    target->base_lo = source->base & 0xFFFF;
    target->base_mid = (source->base >> 16) & 0xFF;
    target->base_hi = (source->base >> 24) & 0xFF;

    target->access = source->access;

    target->limit_hi_flags |= (source->flags << 4);
}


#define GDT_ROWS 6

// todo: better way to allocate these?
uint32_t TSS[32];
GDTRow GDT[GDT_ROWS];
GDTPtr gp;

extern void load_gdt();

void gdt_init(void) {
    terminalInit();
    // todo: maybe diff for 64-bit?
    uint32_t tssRef = (uint32_t) TSS;
    GDTContents rows[GDT_ROWS] = {
        {0,      0x00000,     0x00, 0x0}, // null descriptor
        {0,      0xFFFFF,     0x9A, 0xC}, // kernel code segment
        {0,      0xFFFFF,     0x92, 0xC}, // kernel data segment
        {0,      0xFFFFF,     0xFA, 0xC}, // user code segment
        {0,      0xFFFFF,     0xF2, 0xC}, // user data segment
        {tssRef, sizeof(TSS), 0x89, 0x0}  // task state segment
    };

    for(uint32_t i = 0; i < GDT_ROWS; ++i) {
        fill_gdt_row(GDT+i, rows+i);
    }

    gp.base = (void*) GDT;
    gp.limit = GDT_ROWS - 1;
    terminalPrintf("gdt dump:\n");
    for(uint32_t i = 0; i < GDT_ROWS; ++i) {
        uint8_t* row = (uint32_t*) (GDT+i);
        for(uint32_t j = 0; j < 8; ++j) {
            if(row[j] < 16) terminalPrintf("0");
            terminalPrintf("%x", (uint32_t) (row[j]));
        }
        terminalPrintf("\n");
    }
    load_gdt();
}

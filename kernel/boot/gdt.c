#include <stddef.h>
#include <stdint.h>
#include "include/gdt.h"
#include "../vga/include/vga.h"

void fill_gdt_row(GDTRow* target, GDTContents* source) {
    if(source->limit > 0xFFFFF) {
        // todo: write a kerror() function for this
        kerror("Invalid GDT limit %d\n", source->limit);
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

void gdt_init(void) {
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
    gp.limit = (GDT_ROWS * sizeof(GDTRow)) - 1;
    load_gdt();
}

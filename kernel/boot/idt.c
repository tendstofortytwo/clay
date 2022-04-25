#include "include/idt.h"
#include "../vga/include/vga.h"

__attribute__((interrupt)) void helloWorld(IDTFrame* frame) {
    terminalSetColor(vgaColor(VGA_GREEN, VGA_BLACK));
    kprintf("Hello from an interrupt!\n");
    kprintf("Got ip=%x, cs=%x, flags=%b\n", frame->ip, frame->cs, frame->flags);
    terminalSetColor(VGA_DEFAULT_ENTRYCOLOR);
}

__attribute__((interrupt)) void helloWorldErr(IDTFrameErr* frame) {
    terminalSetColor(vgaColor(VGA_GREEN, VGA_BLACK));
    kprintf("Hello from an error-based interrupt!\n");
    kprintf("Got ip=%x, cs=%x, flags=%b, err=%x\n", frame->ip, frame->cs, frame->flags, frame->err);
    terminalSetColor(VGA_DEFAULT_ENTRYCOLOR);
}

void fill_idt_row(IDTRow* target, IDTContents* source) {
    target->offset_lo = source->offset & 0xFFFF;
    target->offset_hi = (source->offset >> 16) & 0xFFFF;
    target->segment = source->segment;
    target->flags = 0x80; // set present bit
    target->flags |= (source->ring & 0x3) << 5;
    target->flags |= (source->gate & 0xF);
    target->reserved = 0;
}

#define IDT_ROWS 256

__attribute__((aligned(0x10))) IDTRow IDT[IDT_ROWS];
IDTPtr idtPtr;

void idt_init(void) {
    IDTContents rows[] = {
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorldErr, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorldErr, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorldErr, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorldErr, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorldErr, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorldErr, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorldErr, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorldErr, 0x08, INT_GATE32, 0 },
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },
    };

    int validRows = sizeof(rows) / sizeof(IDTRow);

    for(int i = 0; i < validRows; ++i) {
        fill_idt_row(IDT+i, rows+i);
    }
    IDTContents blankRow = {0, 0, 0, 0};
    for(int i = validRows; i < IDT_ROWS; ++i) {
        fill_idt_row(IDT+i, &blankRow);
    }

    idtPtr.offset = &IDT;
    idtPtr.size = (IDT_ROWS * sizeof(IDTRow)) - 1;

    load_idt();
}

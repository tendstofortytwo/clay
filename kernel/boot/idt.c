#include "include/idt.h"
#include "../vga/include/vga.h"
#include "include/pic.h"
#include "include/pit.h"

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
    asm volatile("cli; hlt");
}

__attribute__((interrupt)) void breakPoint(void* ptr) {
    terminalSetColor(vgaColor(VGA_CYAN, VGA_BLACK));
    kprintf("Hello from a breakpoint!\n");
    terminalSetColor(VGA_DEFAULT_ENTRYCOLOR);
    pic_eoi(0);
    io_wait();
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
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },         // 0
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },         // 1
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },         // 2
        { (uint32_t) breakPoint, 0x08, INT_GATE32, 0 },         // 3
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },         // 4
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },         // 5
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },         // 6
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },         // 7
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },      // 8
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },         // 9
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },      // 10
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },      // 11
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },      // 12
        { (uint32_t) helloWorldErr, 0x08, INT_GATE32, 0 },      // 13
        { (uint32_t) helloWorldErr, 0x08, INT_GATE32, 0 },      // 14
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },         // 15
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },         // 16
        { (uint32_t) helloWorldErr, 0x08, INT_GATE32, 0 },      // 17
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },         // 18
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },         // 19
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },         // 20
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },         // 21
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },         // 22
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },         // 23
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },         // 24
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },         // 25
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },         // 26
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },         // 27
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },         // 28
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },         // 29
        { (uint32_t) helloWorldErr, 0x08, INT_GATE32, 0 },      // 30
        { (uint32_t) helloWorld, 0x08, INT_GATE32, 0 },         // 31
        { (uint32_t) timerTick, 0x08, INT_GATE32, 0 },          // 32
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

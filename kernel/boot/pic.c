#include <stdint.h>
#include "include/pic.h"

void pic_remap() {
    uint8_t oldMask1 = inb(PIC1_DATA);
    uint8_t oldMask2 = inb(PIC2_DATA);

    // ICW means Initialization Command Word -- it's the init byte and the
    // three special bytes we send after that to program the PIC

    // ICW1: init
    outb(PIC1_CMD, CMD_INIT);
    io_wait();
    outb(PIC2_CMD, CMD_INIT);
    io_wait();

    // ICW2: which IRQs the interrupts must be raised at
    // Default is 0-7 and 8-f but those are used for other things in protected
    // mode. We send the first IRQ (must be divisible by 8) and the 8 IRQs from
    // there are used.
    outb(PIC1_DATA, PIC1_OFFSET);
    io_wait();
    outb(PIC2_DATA, PIC2_OFFSET);
    io_wait();

    // ICW3: establish dom/sub relationship between the PICs
    // PIC1 is dom, connected directly to CPU's interrupt line
    // PIC2 is sub, connected to IRQ2 on PIC1
    outb(PIC1_DATA, 4); // magic number to tell dom at sub is at IRQ2
    io_wait();
    outb(PIC2_DATA, 2); // magic number to tell sub it is in cascade mode
    io_wait();

    // ICW4: tell PICs what mode to run in
    // we run in 8086 mode
    outb(PIC1_DATA, MODE_8086);
    io_wait();
    outb(PIC2_DATA, MODE_8086);
    io_wait();

    // restore saved masks
    outb(PIC1_DATA, oldMask1);
    outb(PIC2_DATA, oldMask2);
}

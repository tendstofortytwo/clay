#ifndef CLAY_PIC_H
#define CLAY_PIC_H

#include <stdint.h>
#include "portio.h"

#define PIC1        0x20
#define PIC1_CMD    PIC1
#define PIC1_DATA   (PIC1+1)
#define PIC1_OFFSET 0x20

#define PIC2        0xA0
#define PIC2_CMD    PIC2
#define PIC2_DATA   (PIC2+1)
#define PIC2_OFFSET 0x28

#define CMD_INIT    0x11
#define MODE_8086   0x01
#define PIC_EOI     0x20

// EOI -> end of interrupt
static inline void pic_eoi(uint8_t irq) {
	if(irq >= 8)
		outb(PIC2_CMD, PIC_EOI);
 
	outb(PIC1_CMD, PIC_EOI);
}

#endif

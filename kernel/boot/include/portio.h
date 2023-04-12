#ifndef CLAY_PORTIO_H
#define CLAY_PORTIO_H

#include <stdint.h>

static inline void outb(uint16_t port, uint8_t val) {
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// port 0x80 is used by BIOS for POST codes, unused after boot,
// so implement wait by sending a blank value to unused port
static inline void io_wait() {
    outb(0x80, 0);
}

#endif

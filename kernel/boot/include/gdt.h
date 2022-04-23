#ifndef CLAY_GDT_H
#define CLAY_GDT_H

#include <stddef.h>
#include <stdint.h>

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

extern void load_gdt();

#endif

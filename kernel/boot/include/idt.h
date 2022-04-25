#ifndef CLAY_IDT_H
#define CLAY_IDT_H

#include <stdint.h>

#define TASK_GATE 0x5
#define INT_GATE16 0x6
#define TRAP_GATE16 0x7
#define INT_GATE32 0xE
#define TRAP_GATE32 0xF

typedef struct {
    uint32_t offset;
    uint16_t segment;
    uint8_t gate;
    uint8_t ring;
} IDTContents;

typedef struct {
    uint16_t offset_lo;
    uint16_t segment;
    uint8_t reserved;
    uint8_t flags; // contains gate and ring
    uint16_t offset_hi;
} __attribute__((packed)) IDTRow;

typedef struct {
    uint16_t size;
    void* offset;
} __attribute__((packed)) IDTPtr;

typedef struct {
    uint32_t ip;
    uint32_t cs;
    uint32_t flags;
    /* these two are only available if the interrupt causes a change in privilege level */
    uint32_t sp;
    uint32_t ss;
} __attribute__((packed)) IDTFrame;

typedef struct {
    uint32_t err;
    uint32_t ip;
    uint32_t cs;
    uint32_t flags;
    /* these two are only available if the interrupt causes a change in privilege level */
    uint32_t sp;
    uint32_t ss;
} __attribute__((packed)) IDTFrameErr;

extern void load_idt();

#endif

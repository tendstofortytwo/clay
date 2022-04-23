#ifndef CLAY_VGA_H
#define CLAY_VGA_H

#include <stddef.h>
#include <stdint.h>

enum VGAColor {
    VGA_BLACK = 0,
    VGA_BLUE = 1,
    VGA_GREEN = 2,
    VGA_CYAN = 3,
    VGA_RED = 4,
    VGA_MAGENTA = 5,
    VGA_BROWN = 6,
    VGA_LIGHT_GREY = 7,
    VGA_DARK_GREY = 8,
    VGA_LIGHT_BLUE = 9,
    VGA_LIGHT_GREEN = 10,
    VGA_LIGHT_CYAN = 11,
    VGA_LIGHT_RED = 12,
    VGA_LIGHT_MAGENTA = 13,
    VGA_LIGHT_BROWN = 14,
    VGA_WHITE = 15
};

// a color is a byte - first four bits are the background,
// next four bits are the foreground
static inline uint8_t vgaColor(enum VGAColor fg, enum VGAColor bg) {
    return (bg << 4) | fg;
}

static const uint8_t VGA_DEFAULT_ENTRYCOLOR = (VGA_BLACK << 4) | VGA_LIGHT_GREY;

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

void kterminit();
void terminalSetColor(uint8_t);
void kputchar(char);
void kputs(const char *);
void kprintf(const char *, ...);
void kcprintf(uint8_t, const char *, ...);
void kerror(const char *, ...);

#endif

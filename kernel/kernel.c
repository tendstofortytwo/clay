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

#if !defined(__i386__)
#error Please compile with an ix86-elf compiler!
#endif

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
static inline uint8_t vgaEntryColor(enum VGAColor fg, enum VGAColor bg) {
    return (bg << 4) | fg;
}

// an "entry" is two bytes - first byte is the color,
// second byte is the entry
static inline uint16_t vgaEntry(unsigned char ch, uint8_t color) {
    return (uint16_t) ch | (uint16_t) color << 8;
}

size_t strlen(const char* str) {
    size_t len = 0;
    while(str[len]) ++len;
    return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminalRow;
size_t terminalCol;
uint8_t terminalColor;
uint16_t* terminalBuffer;

void terminalPutAtCoords(char c, uint8_t color, size_t x, size_t y) {
    const size_t idx = (y * VGA_WIDTH) + x;
    terminalBuffer[idx] = vgaEntry(c, color);
}

void terminalInit() {
    terminalRow = 0;
    terminalCol = 0;
    terminalColor = vgaEntryColor(VGA_LIGHT_CYAN, VGA_BLACK);
    terminalBuffer = (uint16_t*) 0xb8000;

    for(size_t y = 0; y < VGA_HEIGHT; ++y) {
        for(size_t x = 0; x < VGA_WIDTH; ++x) {
            terminalPutAtCoords(' ', terminalColor, x, y);
        }
    }
}

void terminalSetColor(uint8_t color) {
    terminalColor = color;
}

void terminalPutChar(char c) {
    terminalPutAtCoords(c, terminalColor, terminalCol, terminalRow);

    if(++terminalCol == VGA_WIDTH) {
        terminalCol = 0;
        if(++terminalRow == VGA_HEIGHT) {
            terminalRow = 0;
        }
    }
}

void terminalWrite(const char *s, size_t l) {
    for(size_t i = 0; i < l; ++i)
        terminalPutChar(s[i]);
}

void terminalWriteStr(const char *s) {
    terminalWrite(s, strlen(s));
}

void kernel_main(void) {
    terminalInit();

    terminalWriteStr("Hello, world!\n");
}

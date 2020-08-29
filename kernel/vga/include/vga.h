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

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

static inline uint8_t vgaEntryColor(enum VGAColor, enum VGAColor);
static inline uint16_t vgaEntry(unsigned char, uint8_t);

void terminalInit();
void terminalPutAtCoords(char, uint8_t, size_t, size_t);
char terminalCharFromCoords(size_t, size_t);
void terminalSetColor(uint8_t);
void terminalNextLine();
void terminalPutChar(char);
void terminalWrite(const char *, size_t);
void terminalWriteStr(const char *);
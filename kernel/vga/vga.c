#include <stddef.h>
#include <stdarg.h>
#include "include/vga.h"

size_t terminalRow;
size_t terminalCol;
uint8_t terminalColor;
uint16_t* terminalBuffer;


// need a better place for this
size_t strlen(const char* str) {
    size_t len = 0;
    while(str[len]) ++len;
    return len;
}

// an "entry" is two bytes - first byte is the color,
// second byte is the entry
static inline uint16_t vgaEntry(unsigned char ch, uint8_t color) {
    return (uint16_t) ch | (uint16_t) color << 8;
}

void terminalPutAtCoords(char c, uint8_t color, size_t x, size_t y) {
    const size_t idx = (y * VGA_WIDTH) + x;
    terminalBuffer[idx] = vgaEntry(c, color);
}

char terminalCharFromCoords(size_t x, size_t y) {
    const size_t idx = (y * VGA_WIDTH) + x;
    return (char) (terminalBuffer[idx] & 0x00ff);
}

void terminalInit() {
    terminalRow = 0;
    terminalCol = 0;
    terminalColor = vgaEntryColor(VGA_LIGHT_GREY, VGA_BLACK);
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

void terminalNextLine() {
    // if we are at the last line and we
    // want to go to the "next line", we
    // loop through each row other than 
    // the first, and copy it into the 
    // row above it. then we overwrite
    // the last row (which should have
    // been duplicated into second last)
    // with empty characters
    if(terminalRow == VGA_HEIGHT - 1) {
        for(size_t y = 1; y <= terminalRow; ++y) {
            for(size_t x = 0; x < VGA_WIDTH; ++x) {
                char c = terminalCharFromCoords(x, y);
                terminalPutAtCoords(c, terminalColor, x, y - 1);
            }
        }
        for(size_t x = 0; x < VGA_WIDTH; ++x) {
            terminalPutAtCoords(' ', terminalColor, x, terminalRow);
        }
    }
    // if we are not at the last line, just
    // go to the next line
    else {
        ++terminalRow;
    }

    terminalCol = 0;
}

void terminalPutChar(char c) {
    if(c == '\n') {
        terminalNextLine();
    }
    else {
        terminalPutAtCoords(c, terminalColor, terminalCol, terminalRow);

        if(++terminalCol == VGA_WIDTH) {
            terminalNextLine();
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

void terminalWriteUnsignedNumber(uint32_t n, uint32_t base) {
    if(n == 0) {
        terminalPutChar('0');
        return;
    }

    // should be enough to fit 32-bit INT_MAX
    // there may be better ways to do this
    int8_t digits[10];
    int8_t dCount = 0;

    while(n != 0) {
        digits[dCount++] = (n % base);
        n /= base;
    }

    while(dCount > 0) {
        char ch = digits[--dCount];
        if(ch < 10) ch += '0';
        else ch = (ch - 10) + 'a';
        terminalPutChar(ch);
    }
}

void terminalWriteNumber(int32_t n, uint32_t base) {
    if(n < 0) {
        n = -n;
        terminalPutChar('-');
    }

    terminalWriteUnsignedNumber(n, base);
}

/*
custom printf-like function
supports following format specifiers
 * `%d` - decimal int [signed]
 * `%i` - decimal int [signed]
 * `%u` - decimal int [unsigned]
 * `%b` - binary int [unsigned]
 * `%o` - octal int [unsigned]
 * `%x` - hex int [unsigned]
 * `%c` - character
 * `%s` - string
this should be enough to get us to a libc, where
we can use an actual printf
*/
void terminalPrintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while(*fmt != '\0') {
        if(*fmt == '%') {
            char next = *(fmt+1);
            if(next == '%') {
                terminalPutChar(next);
            }
            else if(next == 'd' || next == 'i') {
                int32_t i = va_arg(args, int32_t);
                terminalWriteNumber(i, 10);
            }
            else if(next == 'u') {
                uint32_t i = va_arg(args, uint32_t);
                terminalWriteUnsignedNumber(i, 10);
            }
            else if(next == 'x') {
                uint32_t i = va_arg(args, uint32_t);
                terminalWriteUnsignedNumber(i, 16);
            }
            else if(next == 'b') {
                uint32_t i = va_arg(args, uint32_t);
                terminalWriteUnsignedNumber(i, 2);
            }
            else if(next == 'o') {
                uint32_t i = va_arg(args, uint32_t);
                terminalWriteUnsignedNumber(i, 8);
            }
            else if(next == 'c') {
                int32_t c = va_arg(args, int32_t);
                terminalPutChar(c);
            }
            else if(next == 's') {
                char *s = va_arg(args, char*);
                terminalWriteStr(s);
            }
            else {
                terminalPutChar('%');
                terminalPutChar(next);
            }
            fmt += 2;
        }
        else {
            terminalPutChar(*fmt);
            ++fmt;
        }
    }
}

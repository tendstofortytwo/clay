#include "include/portio.h"
#include "include/pic.h"
#include "include/pit.h"
#include "../vga/include/vga.h"

int x = 0;

__attribute__((interrupt)) void timerTick(void* ptr) {
    kcprintf(vgaColor(VGA_CYAN, VGA_BLACK), "Hello from the timer! %d\n", x);
    ++x;
    pic_eoi(0);
    io_wait();
}

void pit_init(uint32_t freq) {
    kprintf("pit_init(%x)\n", freq);
    if(freq > PIT_MAX_FREQUENCY) freq = PIT_MAX_FREQUENCY;

    uint32_t reloadValue = PIT_MAX_FREQUENCY / freq;
    if(reloadValue > UINT16_MAX) reloadValue = UINT16_MAX + 1;

    // in mode 2, the reload value is not allowed to be 1
    if(reloadValue == 1) ++reloadValue;

    uint8_t mode = (PITChannel0 << 6) + (PITLoHiBytes << 4) + (PITRateGenerator << 1);
    outb(PIT_MODECOMMAND, mode);
    outb(PIT_CHANNEL0_DATA, reloadValue & 0xff);
    outb(PIT_CHANNEL0_DATA, (reloadValue >> 8) & 0xff);
}

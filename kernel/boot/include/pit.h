#ifndef CLAY_PIT_H
#define CLAY_PIT_H

#include <stdint.h>

#define PIT_CHANNEL0_DATA   0x40
#define PIT_CHANNEL1_DATA   0x41
#define PIT_CHANNEL2_DATA   0x42
#define PIT_MODECOMMAND     0x43

#define PIT_MAX_FREQUENCY   1193182

enum PITChannel {
    PITChannel0,
    PITChannel1,
    PITChannel2,
    PITReadBack
};

enum PITAccessMode {
    PITCounterLatch,
    PITLoByteOnly,
    PITHiByteOnly,
    PITLoHiBytes
};

enum PITOperatingMode {
    PITInterruptOnTerminalCount,
    PITHardwareRetriggerableOneShot,
    PITRateGenerator,
    PITSquareWaveGenerator,
    PITSoftwareTriggeredStrobe,
    PITHardwareTriggeredStrobe
};

__attribute__((interrupt)) void timerTick(void* ptr);

#endif

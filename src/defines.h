#ifndef DEFINES_H
#define DEFINES_H

#include <stdint.h>

#define NESTEST_MODE true

enum CPU_MODE {
    ABSOLUTE = 1,
    ABSOLUTE_X = 2,
    ABSOLUTE_X_PC = 3,
    ABSOLUTE_Y = 4,
    ABSOLUTE_Y_PC = 5,
    ACCUMULATOR = 6,
    IMMEDIATE = 7,
    INDIRECT = 8,
    INDIRECT_X = 9,
    INDIRECT_Y = 10,
    INDIRECT_Y_PC = 11,
    ZERO_PAGE = 12,
    ZERO_PAGE_X = 13,
    ZERO_PAGE_Y = 14
};

enum CPU_FLAG {
    N = 0x80,
    V = 0x40,
    P = 0x20,
    B = 0x10,
    D = 0x8,
    I = 0x4,
    Z = 0x2,
    C = 0x1
};

typedef union __PAIR_REGISTER {
    struct {
        uint8_t l;
        uint8_t h;
    };
    uint16_t w;
} PAIR_REGISTER;

#endif

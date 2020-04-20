#ifndef DEFINES_H
#define DEFINES_H

#include <stdint.h>
#include <iostream>

#define NESTEST_MODE false

enum CPU_MODE {
    ABSOLUTE = 1,
    ABSOLUTE_X = 2,
    ABSOLUTE_X_PC = 3,
    ABSOLUTE_Y = 4,
    ABSOLUTE_Y_PC = 5,
    ACCUMULATOR = 6,
    IMMEDIATE = 7,
    IMPLIED = 8,
    INDIRECT = 9,
    INDIRECT_X = 10,
    INDIRECT_Y = 11,
    INDIRECT_Y_PC = 12,
    RELATIVE = 13,
    ZERO_PAGE = 14,
    ZERO_PAGE_X = 15,
    ZERO_PAGE_Y = 16
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

typedef struct __CPU_OPCODE {
    char name[4];
    CPU_MODE mode;
    int size;
    int cycles;
} CPU_OPCODE;

typedef union __PPU_REGISTER_0 {
    struct {
        unsigned nameTable : 2;
        unsigned increment : 1;
        unsigned sprPattern : 1;
        unsigned bkgPattern : 1;
        unsigned sprSize : 1;
        unsigned selector : 1;
        unsigned nmi : 1;
    };
    uint8_t value;
} PPU_REGISTER_0;

typedef union __PPU_REGISTER_1 {
    struct {
        unsigned greyscale : 1;
        unsigned bkgClip : 1;
        unsigned sprClip : 1;
        unsigned bkgVisible : 1;
        unsigned sprVisible : 1;
        unsigned red : 1;
        unsigned green : 1;
        unsigned blue : 1;
    };
    uint8_t value;
} PPU_REGISTER_1;

typedef union __PPU_REGISTER_2 {
    struct {
        unsigned openBus : 5;
        unsigned sprOverflow : 1;
        unsigned sprZero : 1;
        unsigned vblank : 1;
    };
    uint8_t value;
} PPU_REGISTER_2;

typedef union __PPU_REGISTER_LOOPY {
    struct {
        unsigned coarseX : 5;
        unsigned coarseY : 5;
        unsigned nameTable : 2;
        unsigned fineY : 3;
    };
    unsigned addr : 14;
    unsigned value : 15;
} PPU_REGISTER_LOOPY;

typedef struct __PPU_REGISTERS {
    PPU_REGISTER_0 r0;
    PPU_REGISTER_1 r1;
    PPU_REGISTER_2 r2;
    PPU_REGISTER_LOOPY r5;
    PPU_REGISTER_LOOPY r6;
} PPU_REGISTERS;

#endif

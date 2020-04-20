#pragma once
#ifndef DEFINES_H
#define DEFINES_H

#include <stdint.h>
#include <iostream>
#include "include/SDL.h"

#define NESTEST_MODE false
#define DEBUG_MODE true

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

typedef struct __EMULATOR_WINDOW {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* display;
    int width;
    int height;
} EMULATOR_WINDOW;

enum NT_MIRRORING {
    HORIZONTAL,
    VERTICAL
};

enum PPU_SCANLINE {
    POST_RENDER,
    PRE_RENDER,
    VBLANK,
    VISIBLE
};

typedef struct __PPU_PIPELINE {
    uint16_t bkgShiftL;
    uint16_t bkgShiftH;
    uint8_t atrShiftL;
    uint8_t atrShiftH;
    uint8_t bkgLatchL;
    uint8_t bkgLatchH;
    bool atrLatchL;
    bool atrLatchH;
} PPU_PIPELINE;

static const uint32_t DEBUGGER_PALETTE[4] = {0x000000, 0xff0000, 0x00ff00, 0x0000ff};

static const uint32_t NES_PALETTE[64] = {
    0x757575, 0x271b8f, 0x0000ab, 0x47009f, 0x8f0077, 0xab0013, 0xa70000, 0x7f0b00,
    0x432f00, 0x004700, 0x005100, 0x003f17, 0x1b3f5f, 0x000000, 0x000000, 0x000000,
    0xbcbcbc, 0x0073ef, 0x233bef, 0x8300f3, 0xbf00bf, 0xe7005b, 0xdb2b00, 0xcb4f0f,
    0x8b7300, 0x009700, 0x00ab00, 0x00933b, 0x00838b, 0x000000, 0x000000, 0x000000,
    0xffffff, 0x3fbfff, 0x5f97ff, 0xa78bfd, 0xf77bff, 0xff77b7, 0xff7763, 0xff9b3b,
    0xf3bf3f, 0x83d313, 0x4fdf4b, 0x58f898, 0x00ebdb, 0x000000, 0x000000, 0x000000,
    0xffffff, 0xabe7ff, 0xc7d7ff, 0xd7cbff, 0xffc7ff, 0xffc7db, 0xffbfb3, 0xffdbab,
    0xffe7a3, 0xe3ffa3, 0xabf3bf, 0xb3ffcf, 0x9ffff3, 0x000000, 0x000000, 0x000000
};

#endif

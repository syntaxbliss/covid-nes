#ifndef DEFINES_H
#define DEFINES_H

#include <stdint.h>

#define NESTEST_MODE true

typedef union __PAIR_REGISTER {
    struct {
        uint8_t l;
        uint8_t h;
    };
    uint16_t w;
} PAIR_REGISTER;

#endif

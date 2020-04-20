#pragma once
#ifndef UTILS_H
#define UTILS_H

class Utils {
    public:
        static uint16_t MakeAddress(uint8_t h, uint8_t l) {
            return (((h << 8) | l) & 0xffff);
        }

        static bool TestByteBit(uint8_t value, int bit) {
            uint8_t mask = (1 << bit);

            return ((value & mask) == mask);
        }

        static bool IsByteNegative(uint8_t value) {
            return TestByteBit(value, 7);
        }
};

#endif

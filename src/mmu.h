#pragma once
#ifndef MMU_H
#define MMU_H

#include <stdint.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include "defines.h"
#include "utils.h"

class MMU {
    public:
                        MMU();
        virtual         ~MMU();
        void            allocate(uint8_t*);
        uint8_t         readRom(uint16_t);
        void            writeRom(uint16_t, uint8_t);
        uint8_t         readVram(uint16_t);
        void            writeVram(uint16_t, uint8_t);

    private:
        uint16_t        ntAddress(uint16_t);

    private:
        uint8_t*        rom;
        uint8_t*        vram;
        uint8_t*        sram;
        NT_MIRRORING    ntMirroring;
};

#endif

#ifndef MMU_H
#define MMU_H

#include <stdint.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>

class MMU {
    public:
                    MMU();
        virtual     ~MMU();
        void        allocate(uint8_t*);

    private:
        uint8_t*    rom;
        uint8_t*    vram;
        uint8_t*    sram;
};

#endif

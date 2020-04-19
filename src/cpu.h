#ifndef CPU_H
#define CPU_H

#include "defines.h"
#include <iostream>

class CPU {
    public:
                        CPU(uint8_t (*read)(uint16_t), void (*write)(uint16_t, uint8_t));
        virtual         ~CPU();
        void            reset();
        uint8_t         fetch();
        int             execute(uint8_t);

    private:
        uint8_t         (*read)(uint16_t);
        void            (*write)(uint16_t, uint8_t);
        PAIR_REGISTER   pc;
        uint8_t         a;
        uint8_t         x;
        uint8_t         y;
        uint8_t         sp;
        uint8_t         f;
        int             cycles;
        int             totalCycles;
};

#endif

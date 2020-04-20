#ifndef PPU_H
#define PPU_H

#include <stdint.h>
#include "defines.h"

class PPU {
    public:
                        PPU(uint8_t (*)(uint16_t), void (*)(uint16_t, uint8_t));
        virtual         ~PPU();
        void            update();
        bool            wantNmi();
        bool            readyToRender();
        uint8_t         readRegister(int);
        void            writeRegister(int, uint8_t);

    private:
        uint8_t         (*readVram)(uint16_t);
        void            (*writeVram)(uint16_t, uint8_t);
        PPU_REGISTERS   regs;
        int             ticks;
        int             scanline;
        bool            firstRead;
        bool            pendingNmi;
        bool            frameReady;
};

#endif
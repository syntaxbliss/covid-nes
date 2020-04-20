#pragma once
#ifndef PPU_H
#define PPU_H

#include <stdint.h>
#include "defines.h"

class PPU {
    public:
        #if DEBUG_MODE
        uint16_t                    getBasePT();
        #endif
                                    PPU(uint8_t (*)(uint16_t), void (*)(uint16_t, uint8_t));
        virtual                     ~PPU();
        void                        update(int);
        bool                        wantNmi();
        bool                        readyToRender();
        uint8_t                     readRegister(int);
        void                        writeRegister(int, uint8_t);

    private:
        bool                        isRendering();
        template<PPU_SCANLINE> void stepScanline();
        void                        updatePipeline();
        uint16_t                    ntAddress();
        uint16_t                    atrAddress();
        uint16_t                    ptAddress();
        void                        horizontalScroll();
        void                        verticalScroll();
        void                        horizontalReload();
        void                        verticalReload();

    private:
        uint8_t                     (*readVram)(uint16_t);
        void                        (*writeVram)(uint16_t, uint8_t);
        PPU_REGISTERS               regs;
        uint8_t                     fineX;
        int                         ticks;
        int                         scanline;
        bool                        firstRead;
        bool                        pendingNmi;
        bool                        frameReady;
        bool                        isOddFrame;
        uint16_t                    renderAddress;
        PPU_PIPELINE                pipeline;
        uint8_t                     tileNumber;
        uint8_t                     attributeByte;
        uint8_t                     delayedBuffer;
};

#endif

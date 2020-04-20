#pragma once
#ifndef NES_H
#define NES_H

#include "mmu.h"
#include "cpu.h"
#include "ppu.h"
#include "debugger.h"

class NES {
    public:
        static NES*     GetInstance();
        virtual         ~NES();
        void            loadFile(const char *);
        void            start();
        uint8_t         readRom(uint16_t);
        void            writeRom(uint16_t, uint8_t);
        uint8_t         readVram(uint16_t);
        void            writeVram(uint16_t, uint8_t);
        void            launchGraphics();
        void            teardownGraphics();

    private:
                        NES();
        void            handleInterrupts();
        void            drawScreen();

    private:
        #if DEBUG_MODE
        Debugger*           debugger;
        #endif
        static NES*         Instance;
        bool                running;
        MMU*                mmu;
        CPU*                cpu;
        PPU*                ppu;
        EMULATOR_WINDOW*    screen;
        int*                screenBuffer;
};

#endif

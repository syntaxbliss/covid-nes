#pragma once
#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "mmu.h"
#include "ppu.h"
#include "defines.h"
#include <cmath>

class Debugger {
    public:
                            Debugger(MMU*, PPU*);
        virtual             ~Debugger();
        void                launchGraphics();
        void                teardownGraphics();
        void                drawPatterns();
        void                drawNameTables();

    private:
        MMU*                mmu;
        PPU*                ppu;
        EMULATOR_WINDOW*    patterns;
        EMULATOR_WINDOW*    nameTables;
};

#endif

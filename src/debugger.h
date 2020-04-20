#pragma once
#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "mmu.h"
#include "defines.h"
#include <cmath>

class Debugger {
    public:
                        Debugger(MMU*);
        virtual         ~Debugger();
        void            launchGraphics();
        void            teardownGraphics();
        void            drawPatterns();
        void            drawNameTables();

    private:
        MMU*            mmu;
        EMULATOR_WINDOW patterns;
};

#endif

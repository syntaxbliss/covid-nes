#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "mmu.h"
#include "defines.h"
#include <cmath>

class Debugger {
    public:
                        Debugger(MMU*);
        virtual         ~Debugger();
        void            drawPatterns();
        void            launchGraphics();
        void            teardownGraphics();

    private:
        MMU*            mmu;
        EMULATOR_WINDOW patterns;
};

#endif

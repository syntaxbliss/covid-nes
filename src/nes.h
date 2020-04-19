#ifndef NES_H
#define NES_H

#include "mmu.h"
#include "cpu.h"

class NES {
    public:
        static NES* GetInstance();
        virtual     ~NES();
        void        loadFile(const char *);
        void        start();
        uint8_t     readRom(uint16_t);
        void        writeRom(uint16_t, uint8_t);

    private:
                    NES();

    private:
        static NES* Instance;
        bool        running;
        MMU*        mmu;
        CPU*        cpu;
};

#endif

#ifndef NES_H
#define NES_H

#include "mmu.h"

class NES {
    public:
        static NES* GetInstance();
        virtual     ~NES();
        void        loadFile(const char *);

    private:
                    NES();

    private:
        static NES* Instance;
        MMU* mmu;
};

#endif

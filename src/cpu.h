#ifndef CPU_H
#define CPU_H

#include "defines.h"
#include <iostream>
#include "utils.h"
#include <map>
#include <cstring>

class CPU {
    public:
                        CPU(uint8_t (*read)(uint16_t), void (*write)(uint16_t, uint8_t));
        virtual         ~CPU();
        void            reset();
        uint8_t         fetch();
        int             execute(uint8_t);

    private:
        #if NESTEST_MODE
        void            trace(uint8_t);
        #endif
        uint16_t        address(CPU_MODE);
        void            clearFlag(CPU_FLAG);
        void            setFlag(CPU_FLAG);
        bool            testFlag(CPU_FLAG);
        void            checkFlag(CPU_FLAG, bool);
        void            stackPush(uint8_t);
        uint8_t         stackPop();
        void            _jump(CPU_MODE, int);
        void            _load(CPU_MODE, uint8_t &, int);
        void            _store(CPU_MODE, uint8_t, int);
        void            _subroutine(int);
        void            _flag(CPU_FLAG, bool, int);
        void            _branch(CPU_FLAG, bool, int);
        void            _bit(CPU_MODE, int);
        void            _returnSub(int);
        void            _push(uint8_t, bool, int);
        void            _pull(uint8_t &, bool, int);
        void            _and(CPU_MODE, int);
        void            _compare(CPU_MODE, uint8_t, int);
        void            _or(CPU_MODE, int);
        void            _xor(CPU_MODE, int);
        void            _adc(CPU_MODE, int);
        void            _sbc(CPU_MODE, int);
        void            _incReg(uint8_t &, int);
        void            _decReg(uint8_t &, int);
        void            _incMem(CPU_MODE, int);
        void            _decMem(CPU_MODE, int);
        void            _transfer(uint8_t, uint8_t &, bool, int);
        void            _returnInt(int);
        void            _shiftRight(CPU_MODE, int);
        void            _shiftLeft(CPU_MODE, int);
        void            _rotateRight(CPU_MODE, int);
        void            _rotateLeft(CPU_MODE, int);

    private:
        #if NESTEST_MODE
        std::map<int, CPU_OPCODE>   opcodesTable;
        #endif
        uint8_t         (*read)(uint16_t);
        void            (*write)(uint16_t, uint8_t);
        PAIR_REGISTER   pc;
        uint8_t         a;
        uint8_t         x;
        uint8_t         y;
        uint8_t         sp;
        uint8_t         f;
        int             ticks;
        int             totalTicks;
};

#endif

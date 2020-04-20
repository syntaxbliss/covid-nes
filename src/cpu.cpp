#include "cpu.h"

CPU::CPU(uint8_t (*readHandler)(uint16_t), void (*writeHandler)(uint16_t, uint8_t)) {
    read = readHandler;
    write = writeHandler;

    #if NESTEST_MODE
    FILE *fp = fopen("/home/lebird/Downloads/output.txt", "wt");
    fclose(fp);

    opcodesTable[0x4c] = CPU_OPCODE {"JMP", CPU_MODE::ABSOLUTE, 3, 3};
    opcodesTable[0x6c] = CPU_OPCODE {"JMP", CPU_MODE::INDIRECT, 3, 5};
    opcodesTable[0xa2] = CPU_OPCODE {"LDX", CPU_MODE::IMMEDIATE, 2, 2};
    opcodesTable[0xa6] = CPU_OPCODE {"LDX", CPU_MODE::ZERO_PAGE, 2, 3};
    opcodesTable[0xb6] = CPU_OPCODE {"LDX", CPU_MODE::ZERO_PAGE_Y, 2, 4};
    opcodesTable[0xae] = CPU_OPCODE {"LDX", CPU_MODE::ABSOLUTE, 3, 4};
    opcodesTable[0xbe] = CPU_OPCODE {"LDX", CPU_MODE::ABSOLUTE_Y_PC, 3, 4};
    opcodesTable[0xa9] = CPU_OPCODE {"LDA", CPU_MODE::IMMEDIATE, 2, 2};
    opcodesTable[0xa5] = CPU_OPCODE {"LDA", CPU_MODE::ZERO_PAGE, 2, 3};
    opcodesTable[0xb5] = CPU_OPCODE {"LDA", CPU_MODE::ZERO_PAGE_X, 2, 4};
    opcodesTable[0xad] = CPU_OPCODE {"LDA", CPU_MODE::ABSOLUTE, 3, 4};
    opcodesTable[0xbd] = CPU_OPCODE {"LDA", CPU_MODE::ABSOLUTE_X_PC, 3, 4};
    opcodesTable[0xb9] = CPU_OPCODE {"LDA", CPU_MODE::ABSOLUTE_Y_PC, 3, 4};
    opcodesTable[0xa1] = CPU_OPCODE {"LDA", CPU_MODE::INDIRECT_X, 2, 6};
    opcodesTable[0xb1] = CPU_OPCODE {"LDA", CPU_MODE::INDIRECT_Y_PC, 2, 5};
    opcodesTable[0xa0] = CPU_OPCODE {"LDY", CPU_MODE::IMMEDIATE, 2, 2};
    opcodesTable[0xa4] = CPU_OPCODE {"LDY", CPU_MODE::ZERO_PAGE, 2, 3};
    opcodesTable[0xb4] = CPU_OPCODE {"LDY", CPU_MODE::ZERO_PAGE_X, 2, 4};
    opcodesTable[0xac] = CPU_OPCODE {"LDY", CPU_MODE::ABSOLUTE, 3, 4};
    opcodesTable[0xbc] = CPU_OPCODE {"LDY", CPU_MODE::ABSOLUTE_X_PC, 3, 4};
    opcodesTable[0x86] = CPU_OPCODE {"STX", CPU_MODE::ZERO_PAGE, 2, 3};
    opcodesTable[0x96] = CPU_OPCODE {"STX", CPU_MODE::ZERO_PAGE_Y, 2, 4};
    opcodesTable[0x8e] = CPU_OPCODE {"STX", CPU_MODE::ABSOLUTE, 3, 4};
    opcodesTable[0x85] = CPU_OPCODE {"STA", CPU_MODE::ZERO_PAGE, 2, 3};
    opcodesTable[0x95] = CPU_OPCODE {"STA", CPU_MODE::ZERO_PAGE_X, 2, 4};
    opcodesTable[0x8d] = CPU_OPCODE {"STA", CPU_MODE::ABSOLUTE, 3, 4};
    opcodesTable[0x9d] = CPU_OPCODE {"STA", CPU_MODE::ABSOLUTE_X, 3, 5};
    opcodesTable[0x99] = CPU_OPCODE {"STA", CPU_MODE::ABSOLUTE_Y, 3, 5};
    opcodesTable[0x81] = CPU_OPCODE {"STA", CPU_MODE::INDIRECT_X, 2, 6};
    opcodesTable[0x91] = CPU_OPCODE {"STA", CPU_MODE::INDIRECT_Y, 2, 6};
    opcodesTable[0x84] = CPU_OPCODE {"STY", CPU_MODE::ZERO_PAGE, 2, 3};
    opcodesTable[0x94] = CPU_OPCODE {"STY", CPU_MODE::ZERO_PAGE_X, 2, 4};
    opcodesTable[0x8c] = CPU_OPCODE {"STY", CPU_MODE::ABSOLUTE, 3, 4};
    opcodesTable[0x20] = CPU_OPCODE {"JSR", CPU_MODE::ABSOLUTE, 3, 6};
    opcodesTable[0xea] = CPU_OPCODE {"NOP", CPU_MODE::IMPLIED, 1, 2};
    opcodesTable[0x38] = CPU_OPCODE {"SEC", CPU_MODE::IMPLIED, 1, 2};
    opcodesTable[0xb0] = CPU_OPCODE {"BCS", CPU_MODE::RELATIVE, 2, 2};
    opcodesTable[0x18] = CPU_OPCODE {"CLC", CPU_MODE::IMPLIED, 1, 2};
    opcodesTable[0x90] = CPU_OPCODE {"BCC", CPU_MODE::RELATIVE, 2, 2};
    opcodesTable[0xf0] = CPU_OPCODE {"BEQ", CPU_MODE::RELATIVE, 2, 2};
    opcodesTable[0xd0] = CPU_OPCODE {"BNE", CPU_MODE::RELATIVE, 2, 2};
    opcodesTable[0x24] = CPU_OPCODE {"BIT", CPU_MODE::ZERO_PAGE, 2, 3};
    opcodesTable[0x2c] = CPU_OPCODE {"BIT", CPU_MODE::ABSOLUTE, 3, 4};
    opcodesTable[0x70] = CPU_OPCODE {"BVS", CPU_MODE::RELATIVE, 2, 2};
    opcodesTable[0x50] = CPU_OPCODE {"BVC", CPU_MODE::RELATIVE, 2, 2};
    opcodesTable[0x10] = CPU_OPCODE {"BPL", CPU_MODE::RELATIVE, 2, 2};
    opcodesTable[0x60] = CPU_OPCODE {"RTS", CPU_MODE::IMPLIED, 1, 6};
    opcodesTable[0x78] = CPU_OPCODE {"SEI", CPU_MODE::IMPLIED, 1, 2};
    opcodesTable[0xf8] = CPU_OPCODE {"SED", CPU_MODE::IMPLIED, 1, 2};
    opcodesTable[0x08] = CPU_OPCODE {"PHP", CPU_MODE::IMPLIED, 1, 3};
    opcodesTable[0x48] = CPU_OPCODE {"PHA", CPU_MODE::IMPLIED, 1, 3};
    opcodesTable[0x68] = CPU_OPCODE {"PLA", CPU_MODE::IMPLIED, 1, 4};
    opcodesTable[0x28] = CPU_OPCODE {"PLP", CPU_MODE::IMPLIED, 1, 4};
    opcodesTable[0x29] = CPU_OPCODE {"AND", CPU_MODE::IMMEDIATE, 2, 2};
    opcodesTable[0x25] = CPU_OPCODE {"AND", CPU_MODE::ZERO_PAGE, 2, 3};
    opcodesTable[0x35] = CPU_OPCODE {"AND", CPU_MODE::ZERO_PAGE_X, 2, 4};
    opcodesTable[0x2d] = CPU_OPCODE {"AND", CPU_MODE::ABSOLUTE, 3, 4};
    opcodesTable[0x3d] = CPU_OPCODE {"AND", CPU_MODE::ABSOLUTE_X_PC, 3, 4};
    opcodesTable[0x39] = CPU_OPCODE {"AND", CPU_MODE::ABSOLUTE_Y_PC, 3, 4};
    opcodesTable[0x21] = CPU_OPCODE {"AND", CPU_MODE::INDIRECT_X, 2, 6};
    opcodesTable[0x31] = CPU_OPCODE {"AND", CPU_MODE::INDIRECT_Y_PC, 2, 5};
    opcodesTable[0xc9] = CPU_OPCODE {"CMP", CPU_MODE::IMMEDIATE, 2, 2};
    opcodesTable[0xc5] = CPU_OPCODE {"CMP", CPU_MODE::ZERO_PAGE, 2, 3};
    opcodesTable[0xd5] = CPU_OPCODE {"CMP", CPU_MODE::ZERO_PAGE_X, 2, 4};
    opcodesTable[0xcd] = CPU_OPCODE {"CMP", CPU_MODE::ABSOLUTE, 3, 4};
    opcodesTable[0xdd] = CPU_OPCODE {"CMP", CPU_MODE::ABSOLUTE_X_PC, 3, 4};
    opcodesTable[0xd9] = CPU_OPCODE {"CMP", CPU_MODE::ABSOLUTE_Y_PC, 3, 4};
    opcodesTable[0xc1] = CPU_OPCODE {"CMP", CPU_MODE::INDIRECT_X, 2, 6};
    opcodesTable[0xd1] = CPU_OPCODE {"CMP", CPU_MODE::INDIRECT_Y_PC, 2, 5};
    opcodesTable[0xd8] = CPU_OPCODE {"CLD", CPU_MODE::IMPLIED, 1, 2};
    opcodesTable[0x30] = CPU_OPCODE {"BMI", CPU_MODE::RELATIVE, 2, 2};
    opcodesTable[0x09] = CPU_OPCODE {"ORA", CPU_MODE::IMMEDIATE, 2, 2};
    opcodesTable[0x05] = CPU_OPCODE {"ORA", CPU_MODE::ZERO_PAGE, 2, 3};
    opcodesTable[0x15] = CPU_OPCODE {"ORA", CPU_MODE::ZERO_PAGE_X, 2, 4};
    opcodesTable[0x0d] = CPU_OPCODE {"ORA", CPU_MODE::ABSOLUTE, 3, 4};
    opcodesTable[0x1d] = CPU_OPCODE {"ORA", CPU_MODE::ABSOLUTE_X_PC, 3, 4};
    opcodesTable[0x19] = CPU_OPCODE {"ORA", CPU_MODE::ABSOLUTE_Y_PC, 3, 4};
    opcodesTable[0x01] = CPU_OPCODE {"ORA", CPU_MODE::INDIRECT_X, 2, 6};
    opcodesTable[0x11] = CPU_OPCODE {"ORA", CPU_MODE::INDIRECT_Y_PC, 2, 5};
    opcodesTable[0xb8] = CPU_OPCODE {"CLV", CPU_MODE::IMPLIED, 1, 2};
    opcodesTable[0x49] = CPU_OPCODE {"EOR", CPU_MODE::IMMEDIATE, 2, 2};
    opcodesTable[0x45] = CPU_OPCODE {"EOR", CPU_MODE::ZERO_PAGE, 2, 3};
    opcodesTable[0x55] = CPU_OPCODE {"EOR", CPU_MODE::ZERO_PAGE_X, 2, 4};
    opcodesTable[0x4d] = CPU_OPCODE {"EOR", CPU_MODE::ABSOLUTE, 3, 4};
    opcodesTable[0x5d] = CPU_OPCODE {"EOR", CPU_MODE::ABSOLUTE_X_PC, 3, 4};
    opcodesTable[0x59] = CPU_OPCODE {"EOR", CPU_MODE::ABSOLUTE_Y_PC, 3, 4};
    opcodesTable[0x41] = CPU_OPCODE {"EOR", CPU_MODE::INDIRECT_X, 2, 6};
    opcodesTable[0x51] = CPU_OPCODE {"EOR", CPU_MODE::INDIRECT_Y_PC, 2, 5};
    opcodesTable[0x69] = CPU_OPCODE {"ADC", CPU_MODE::IMMEDIATE, 2, 2};
    opcodesTable[0x65] = CPU_OPCODE {"ADC", CPU_MODE::ZERO_PAGE, 2, 3};
    opcodesTable[0x75] = CPU_OPCODE {"ADC", CPU_MODE::ZERO_PAGE_X, 2, 4};
    opcodesTable[0x6d] = CPU_OPCODE {"ADC", CPU_MODE::ABSOLUTE, 3, 4};
    opcodesTable[0x7d] = CPU_OPCODE {"ADC", CPU_MODE::ABSOLUTE_X_PC, 3, 4};
    opcodesTable[0x79] = CPU_OPCODE {"ADC", CPU_MODE::ABSOLUTE_Y_PC, 3, 4};
    opcodesTable[0x61] = CPU_OPCODE {"ADC", CPU_MODE::INDIRECT_X, 2, 6};
    opcodesTable[0x71] = CPU_OPCODE {"ADC", CPU_MODE::INDIRECT_Y_PC, 2, 5};
    opcodesTable[0xc0] = CPU_OPCODE {"CPY", CPU_MODE::IMMEDIATE, 2, 2};
    opcodesTable[0xc4] = CPU_OPCODE {"CPY", CPU_MODE::ZERO_PAGE, 2, 3};
    opcodesTable[0xcc] = CPU_OPCODE {"CPY", CPU_MODE::ABSOLUTE, 3, 4};
    opcodesTable[0xe0] = CPU_OPCODE {"CPX", CPU_MODE::IMMEDIATE, 2, 2};
    opcodesTable[0xe4] = CPU_OPCODE {"CPX", CPU_MODE::ZERO_PAGE, 2, 3};
    opcodesTable[0xec] = CPU_OPCODE {"CPX", CPU_MODE::ABSOLUTE, 3, 4};
    opcodesTable[0xe9] = CPU_OPCODE {"SBC", CPU_MODE::IMMEDIATE, 2, 2};
    opcodesTable[0xe5] = CPU_OPCODE {"SBC", CPU_MODE::ZERO_PAGE, 2, 3};
    opcodesTable[0xf5] = CPU_OPCODE {"SBC", CPU_MODE::ZERO_PAGE_X, 2, 4};
    opcodesTable[0xed] = CPU_OPCODE {"SBC", CPU_MODE::ABSOLUTE, 3, 4};
    opcodesTable[0xfd] = CPU_OPCODE {"SBC", CPU_MODE::ABSOLUTE_X_PC, 3, 4};
    opcodesTable[0xf9] = CPU_OPCODE {"SBC", CPU_MODE::ABSOLUTE_Y_PC, 3, 4};
    opcodesTable[0xe1] = CPU_OPCODE {"SBC", CPU_MODE::INDIRECT_X, 2, 6};
    opcodesTable[0xf1] = CPU_OPCODE {"SBC", CPU_MODE::INDIRECT_Y_PC, 2, 5};
    opcodesTable[0xc8] = CPU_OPCODE {"INY", CPU_MODE::IMPLIED, 1, 2};
    opcodesTable[0xe8] = CPU_OPCODE {"INX", CPU_MODE::IMPLIED, 1, 2};
    opcodesTable[0x88] = CPU_OPCODE {"DEY", CPU_MODE::IMPLIED, 1, 2};
    opcodesTable[0xca] = CPU_OPCODE {"DEX", CPU_MODE::IMPLIED, 1, 2};
    opcodesTable[0xa8] = CPU_OPCODE {"TAY", CPU_MODE::IMPLIED, 1, 2};
    opcodesTable[0xaa] = CPU_OPCODE {"TAX", CPU_MODE::IMPLIED, 1, 2};
    opcodesTable[0x98] = CPU_OPCODE {"TYA", CPU_MODE::IMPLIED, 1, 2};
    opcodesTable[0x8a] = CPU_OPCODE {"TXA", CPU_MODE::IMPLIED, 1, 2};
    opcodesTable[0xba] = CPU_OPCODE {"TSX", CPU_MODE::IMPLIED, 1, 2};
    opcodesTable[0x9a] = CPU_OPCODE {"TXS", CPU_MODE::IMPLIED, 1, 2};
    opcodesTable[0x40] = CPU_OPCODE {"RTI", CPU_MODE::IMPLIED, 1, 6};
    opcodesTable[0x4a] = CPU_OPCODE {"LSR", CPU_MODE::ACCUMULATOR, 1, 2};
    opcodesTable[0x46] = CPU_OPCODE {"LSR", CPU_MODE::ZERO_PAGE, 2, 5};
    opcodesTable[0x56] = CPU_OPCODE {"LSR", CPU_MODE::ZERO_PAGE_X, 2, 6};
    opcodesTable[0x4e] = CPU_OPCODE {"LSR", CPU_MODE::ABSOLUTE, 3, 6};
    opcodesTable[0x5e] = CPU_OPCODE {"LSR", CPU_MODE::ABSOLUTE_X, 3, 7};
    opcodesTable[0x0a] = CPU_OPCODE {"ASL", CPU_MODE::ACCUMULATOR, 1, 2};
    opcodesTable[0x06] = CPU_OPCODE {"ASL", CPU_MODE::ZERO_PAGE, 2, 5};
    opcodesTable[0x16] = CPU_OPCODE {"ASL", CPU_MODE::ZERO_PAGE_X, 2, 6};
    opcodesTable[0x0e] = CPU_OPCODE {"ASL", CPU_MODE::ABSOLUTE, 3, 6};
    opcodesTable[0x1e] = CPU_OPCODE {"ASL", CPU_MODE::ABSOLUTE_X, 3, 7};
    opcodesTable[0x6a] = CPU_OPCODE {"ROR", CPU_MODE::ACCUMULATOR, 1, 2};
    opcodesTable[0x66] = CPU_OPCODE {"ROR", CPU_MODE::ZERO_PAGE, 2, 5};
    opcodesTable[0x76] = CPU_OPCODE {"ROR", CPU_MODE::ZERO_PAGE_X, 2, 6};
    opcodesTable[0x6e] = CPU_OPCODE {"ROR", CPU_MODE::ABSOLUTE, 3, 6};
    opcodesTable[0x7e] = CPU_OPCODE {"ROR", CPU_MODE::ABSOLUTE_X, 3, 7};
    opcodesTable[0x2a] = CPU_OPCODE {"ROL", CPU_MODE::ACCUMULATOR, 1, 2};
    opcodesTable[0x26] = CPU_OPCODE {"ROL", CPU_MODE::ZERO_PAGE, 2, 5};
    opcodesTable[0x36] = CPU_OPCODE {"ROL", CPU_MODE::ZERO_PAGE_X, 2, 6};
    opcodesTable[0x2e] = CPU_OPCODE {"ROL", CPU_MODE::ABSOLUTE, 3, 6};
    opcodesTable[0x3e] = CPU_OPCODE {"ROL", CPU_MODE::ABSOLUTE_X, 3, 7};
    opcodesTable[0xe6] = CPU_OPCODE {"INC", CPU_MODE::ZERO_PAGE, 2, 5};
    opcodesTable[0xf6] = CPU_OPCODE {"INC", CPU_MODE::ZERO_PAGE_X, 2, 6};
    opcodesTable[0xee] = CPU_OPCODE {"INC", CPU_MODE::ABSOLUTE, 3, 6};
    opcodesTable[0xfe] = CPU_OPCODE {"INC", CPU_MODE::ABSOLUTE_X, 3, 7};
    opcodesTable[0xc6] = CPU_OPCODE {"DEC", CPU_MODE::ZERO_PAGE, 2, 5};
    opcodesTable[0xd6] = CPU_OPCODE {"DEC", CPU_MODE::ZERO_PAGE_X, 2, 6};
    opcodesTable[0xce] = CPU_OPCODE {"DEC", CPU_MODE::ABSOLUTE, 3, 6};
    opcodesTable[0xde] = CPU_OPCODE {"DEC", CPU_MODE::ABSOLUTE_X, 3, 7};
    #endif
}

CPU::~CPU() {}

void CPU::reset() {
    #if NESTEST_MODE
    pc.w = 0xc000;
    #else
    pc.l = read(0xfffc);
    pc.h = read(0xfffd);
    #endif

    a = 0;
    x = 0;
    y = 0;
    sp = 0xfd;
    f = 0x24;
    ticks = 0;
    totalTicks = 0;
}

void CPU::nmi() {
    stackPush(pc.h);
    stackPush(pc.l);
    stackPush(f | 0x20);
    setFlag(CPU_FLAG::I);
    pc.l = read(0xfffa);
    pc.h = read(0xfffb);
}

uint8_t CPU::fetch() {
    return read(pc.w++);
}

int CPU::execute(uint8_t opcode) {
    ticks = 0;

    #if NESTEST_MODE
    trace(opcode);
    #endif

    switch (opcode) {
        case 0x4c: _jump(CPU_MODE::ABSOLUTE, 3); break;
        case 0x6c: _jump(CPU_MODE::INDIRECT, 5); break;
        case 0xa9: _load(CPU_MODE::IMMEDIATE, a, 2); break;
        case 0xa5: _load(CPU_MODE::ZERO_PAGE, a, 3); break;
        case 0xb5: _load(CPU_MODE::ZERO_PAGE_X, a, 4); break;
        case 0xad: _load(CPU_MODE::ABSOLUTE, a, 4); break;
        case 0xbd: _load(CPU_MODE::ABSOLUTE_X_PC, a, 4); break;
        case 0xb9: _load(CPU_MODE::ABSOLUTE_Y_PC, a, 4); break;
        case 0xa1: _load(CPU_MODE::INDIRECT_X, a, 6); break;
        case 0xb1: _load(CPU_MODE::INDIRECT_Y_PC, a, 5); break;
        case 0xa2: _load(CPU_MODE::IMMEDIATE, x, 2); break;
        case 0xa6: _load(CPU_MODE::ZERO_PAGE, x, 3); break;
        case 0xb6: _load(CPU_MODE::ZERO_PAGE_Y, x, 4); break;
        case 0xae: _load(CPU_MODE::ABSOLUTE, x, 4); break;
        case 0xbe: _load(CPU_MODE::ABSOLUTE_Y_PC, x, 4); break;
        case 0xa0: _load(CPU_MODE::IMMEDIATE, y, 2); break;
        case 0xa4: _load(CPU_MODE::ZERO_PAGE, y, 3); break;
        case 0xb4: _load(CPU_MODE::ZERO_PAGE_X, y, 4); break;
        case 0xac: _load(CPU_MODE::ABSOLUTE, y, 4); break;
        case 0xbc: _load(CPU_MODE::ABSOLUTE_X_PC, y, 4); break;
        case 0x85: _store(CPU_MODE::ZERO_PAGE, a, 3); break;
        case 0x95: _store(CPU_MODE::ZERO_PAGE_X, a, 4); break;
        case 0x8d: _store(CPU_MODE::ABSOLUTE, a, 4); break;
        case 0x9d: _store(CPU_MODE::ABSOLUTE_X, a, 5); break;
        case 0x99: _store(CPU_MODE::ABSOLUTE_Y, a, 5); break;
        case 0x81: _store(CPU_MODE::INDIRECT_X, a, 6); break;
        case 0x91: _store(CPU_MODE::INDIRECT_Y, a, 6); break;
        case 0x86: _store(CPU_MODE::ZERO_PAGE, x, 3); break;
        case 0x96: _store(CPU_MODE::ZERO_PAGE_Y, x, 4); break;
        case 0x8e: _store(CPU_MODE::ABSOLUTE, x, 4); break;
        case 0x84: _store(CPU_MODE::ZERO_PAGE, y, 3); break;
        case 0x94: _store(CPU_MODE::ZERO_PAGE_X, y, 4); break;
        case 0x8c: _store(CPU_MODE::ABSOLUTE, y, 4); break;
        case 0x20: _subroutine(6); break;
        case 0xea: ticks += 2; break;
        case 0x38: _flag(CPU_FLAG::C, true, 2); break;
        case 0x18: _flag(CPU_FLAG::C, false, 2); break;
        case 0x78: _flag(CPU_FLAG::I, true, 2); break;
        case 0xf8: _flag(CPU_FLAG::D, true, 2); break;
        case 0xd8: _flag(CPU_FLAG::D, false, 2); break;
        case 0xb8: _flag(CPU_FLAG::V, false, 2); break;
        case 0xb0: _branch(CPU_FLAG::C, true, 2); break;
        case 0x90: _branch(CPU_FLAG::C, false, 2); break;
        case 0xf0: _branch(CPU_FLAG::Z, true, 2); break;
        case 0xd0: _branch(CPU_FLAG::Z, false, 2); break;
        case 0x70: _branch(CPU_FLAG::V, true, 2); break;
        case 0x50: _branch(CPU_FLAG::V, false, 2); break;
        case 0x10: _branch(CPU_FLAG::N, false, 2); break;
        case 0x30: _branch(CPU_FLAG::N, true, 2); break;
        case 0x24: _bit(CPU_MODE::ZERO_PAGE, 3); break;
        case 0x2c: _bit(CPU_MODE::ABSOLUTE, 4); break;
        case 0x60: _returnSub(6); break;
        case 0x08: _push(f, true, 3); break;
        case 0x48: _push(a, false, 3); break;
        case 0x68: _pull(a, false, 4); break;
        case 0x28: _pull(f, true, 4); break;
        case 0x29: _and(CPU_MODE::IMMEDIATE, 2); break;
        case 0x25: _and(CPU_MODE::ZERO_PAGE, 3); break;
        case 0x35: _and(CPU_MODE::ZERO_PAGE_X, 4); break;
        case 0x2d: _and(CPU_MODE::ABSOLUTE, 4); break;
        case 0x3d: _and(CPU_MODE::ABSOLUTE_X_PC, 4); break;
        case 0x39: _and(CPU_MODE::ABSOLUTE_Y_PC, 4); break;
        case 0x21: _and(CPU_MODE::INDIRECT_X, 6); break;
        case 0x31: _and(CPU_MODE::INDIRECT_Y_PC, 5); break;
        case 0xc9: _compare(CPU_MODE::IMMEDIATE, a, 2); break;
        case 0xc5: _compare(CPU_MODE::ZERO_PAGE, a, 3); break;
        case 0xd5: _compare(CPU_MODE::ZERO_PAGE_X, a, 4); break;
        case 0xcd: _compare(CPU_MODE::ABSOLUTE, a, 4); break;
        case 0xdd: _compare(CPU_MODE::ABSOLUTE_X_PC, a, 4); break;
        case 0xd9: _compare(CPU_MODE::ABSOLUTE_Y_PC, a, 4); break;
        case 0xc1: _compare(CPU_MODE::INDIRECT_X, a, 6); break;
        case 0xd1: _compare(CPU_MODE::INDIRECT_Y_PC, a, 5); break;
        case 0xe0: _compare(CPU_MODE::IMMEDIATE, x, 2); break;
        case 0xe4: _compare(CPU_MODE::ZERO_PAGE, x, 3); break;
        case 0xec: _compare(CPU_MODE::ABSOLUTE, x, 4); break;
        case 0xc0: _compare(CPU_MODE::IMMEDIATE, y, 2); break;
        case 0xc4: _compare(CPU_MODE::ZERO_PAGE, y, 3); break;
        case 0xcc: _compare(CPU_MODE::ABSOLUTE, y, 4); break;
        case 0x09: _or(CPU_MODE::IMMEDIATE, 2); break;
        case 0x05: _or(CPU_MODE::ZERO_PAGE, 3); break;
        case 0x15: _or(CPU_MODE::ZERO_PAGE_X, 4); break;
        case 0x0d: _or(CPU_MODE::ABSOLUTE, 4); break;
        case 0x1d: _or(CPU_MODE::ABSOLUTE_X_PC, 4); break;
        case 0x19: _or(CPU_MODE::ABSOLUTE_Y_PC, 4); break;
        case 0x01: _or(CPU_MODE::INDIRECT_X, 6); break;
        case 0x11: _or(CPU_MODE::INDIRECT_Y_PC, 5); break;
        case 0x49: _xor(CPU_MODE::IMMEDIATE, 2); break;
        case 0x45: _xor(CPU_MODE::ZERO_PAGE, 3); break;
        case 0x55: _xor(CPU_MODE::ZERO_PAGE_X, 4); break;
        case 0x4d: _xor(CPU_MODE::ABSOLUTE, 4); break;
        case 0x5d: _xor(CPU_MODE::ABSOLUTE_X_PC, 4); break;
        case 0x59: _xor(CPU_MODE::ABSOLUTE_Y_PC, 4); break;
        case 0x41: _xor(CPU_MODE::INDIRECT_X, 6); break;
        case 0x51: _xor(CPU_MODE::INDIRECT_Y_PC, 5); break;
        case 0x69: _adc(CPU_MODE::IMMEDIATE, 2); break;
        case 0x65: _adc(CPU_MODE::ZERO_PAGE, 3); break;
        case 0x75: _adc(CPU_MODE::ZERO_PAGE_X, 4); break;
        case 0x6d: _adc(CPU_MODE::ABSOLUTE, 4); break;
        case 0x7d: _adc(CPU_MODE::ABSOLUTE_X_PC, 4); break;
        case 0x79: _adc(CPU_MODE::ABSOLUTE_Y_PC, 4); break;
        case 0x61: _adc(CPU_MODE::INDIRECT_X, 6); break;
        case 0x71: _adc(CPU_MODE::INDIRECT_Y_PC, 5); break;
        case 0xe9: _sbc(CPU_MODE::IMMEDIATE, 2); break;
        case 0xe5: _sbc(CPU_MODE::ZERO_PAGE, 3); break;
        case 0xf5: _sbc(CPU_MODE::ZERO_PAGE_X, 4); break;
        case 0xed: _sbc(CPU_MODE::ABSOLUTE, 4); break;
        case 0xfd: _sbc(CPU_MODE::ABSOLUTE_X_PC, 4); break;
        case 0xf9: _sbc(CPU_MODE::ABSOLUTE_Y_PC, 4); break;
        case 0xe1: _sbc(CPU_MODE::INDIRECT_X, 6); break;
        case 0xf1: _sbc(CPU_MODE::INDIRECT_Y_PC, 5); break;
        case 0xe8: _incReg(x, 2); break;
        case 0xc8: _incReg(y, 2); break;
        case 0xca: _decReg(x, 2); break;
        case 0x88: _decReg(y, 2); break;
        case 0xe6: _incMem(CPU_MODE::ZERO_PAGE, 5); break;
        case 0xf6: _incMem(CPU_MODE::ZERO_PAGE_X, 6); break;
        case 0xee: _incMem(CPU_MODE::ABSOLUTE, 6); break;
        case 0xfe: _incMem(CPU_MODE::ABSOLUTE_X, 7); break;
        case 0xc6: _decMem(CPU_MODE::ZERO_PAGE, 5); break;
        case 0xd6: _decMem(CPU_MODE::ZERO_PAGE_X, 6); break;
        case 0xce: _decMem(CPU_MODE::ABSOLUTE, 6); break;
        case 0xde: _decMem(CPU_MODE::ABSOLUTE_X, 7); break;
        case 0xa8: _transfer(a, y, true, 2); break;
        case 0xaa: _transfer(a, x, true, 2); break;
        case 0x98: _transfer(y, a, true, 2); break;
        case 0x8a: _transfer(x, a, true, 2); break;
        case 0xba: _transfer(sp, x, true, 2); break;
        case 0x9a: _transfer(x, sp, false, 2); break;
        case 0x40: _returnInt(6); break;
        case 0x4a: _shiftRight(CPU_MODE::ACCUMULATOR, 2); break;
        case 0x46: _shiftRight(CPU_MODE::ZERO_PAGE, 5); break;
        case 0x56: _shiftRight(CPU_MODE::ZERO_PAGE_X, 6); break;
        case 0x4e: _shiftRight(CPU_MODE::ABSOLUTE, 6); break;
        case 0x5e: _shiftRight(CPU_MODE::ABSOLUTE_X, 7); break;
        case 0x0a: _shiftLeft(CPU_MODE::ACCUMULATOR, 2); break;
        case 0x06: _shiftLeft(CPU_MODE::ZERO_PAGE, 5); break;
        case 0x16: _shiftLeft(CPU_MODE::ZERO_PAGE_X, 6); break;
        case 0x0e: _shiftLeft(CPU_MODE::ABSOLUTE, 6); break;
        case 0x1e: _shiftLeft(CPU_MODE::ABSOLUTE_X, 7); break;
        case 0x6a: _rotateRight(CPU_MODE::ACCUMULATOR, 2); break;
        case 0x66: _rotateRight(CPU_MODE::ZERO_PAGE, 5); break;
        case 0x76: _rotateRight(CPU_MODE::ZERO_PAGE_X, 6); break;
        case 0x6e: _rotateRight(CPU_MODE::ABSOLUTE, 6); break;
        case 0x7e: _rotateRight(CPU_MODE::ABSOLUTE_X, 7); break;
        case 0x2a: _rotateLeft(CPU_MODE::ACCUMULATOR, 2); break;
        case 0x26: _rotateLeft(CPU_MODE::ZERO_PAGE, 5); break;
        case 0x36: _rotateLeft(CPU_MODE::ZERO_PAGE_X, 6); break;
        case 0x2e: _rotateLeft(CPU_MODE::ABSOLUTE, 6); break;
        case 0x3e: _rotateLeft(CPU_MODE::ABSOLUTE_X, 7); break;

        default: {
            std::printf("CPU :: invalid opcode %02X at address %04X", opcode, (pc.w - 1) & 0xffff);
            std::cin.get();
            exit(1);
        }
    }

    totalTicks += (ticks * 3);

    if (! ticks) {
        std::printf("CPU :: cycles not updated for opcode %02X", opcode);
        std::cin.get();
        exit(1);
    }

    return ticks;
}

void CPU::clearFlag(CPU_FLAG flag) {
    f &= (~flag);
}

void CPU::setFlag(CPU_FLAG flag) {
    f |= flag;
}

bool CPU::testFlag(CPU_FLAG flag) {
    return ((f & flag) == flag);
}

void CPU::checkFlag(CPU_FLAG flag, bool value) {
    if (value) {
        setFlag(flag);
    } else {
        clearFlag(flag);
    }
}

void CPU::stackPush(uint8_t value) {
    write(sp + 0x100, value);
    sp--;
}

uint8_t CPU::stackPop() {
    sp++;

    return read(sp + 0x100);
}

uint16_t CPU::address(CPU_MODE mode) {
    switch (mode) {
        case CPU_MODE::ABSOLUTE: {
            uint8_t l = fetch();
            uint8_t h = fetch();

            return Utils::MakeAddress(h, l);
        }

        case ABSOLUTE_X: case ABSOLUTE_X_PC: {
            uint8_t l = fetch();
            uint8_t h = fetch();
            uint16_t before = Utils::MakeAddress(h, l);
            uint16_t addr = (before + x) & 0xffff;

            if ((mode == ABSOLUTE_X_PC) && ((before & 0xff00) != (addr & 0xff00))) {
                ticks++;
            }

            return addr;
        }

        case ABSOLUTE_Y: case ABSOLUTE_Y_PC: {
            uint8_t l = fetch();
            uint8_t h = fetch();
            uint16_t before = Utils::MakeAddress(h, l);
            uint16_t addr = (before + y) & 0xffff;

            if ((mode == ABSOLUTE_Y_PC) && ((before & 0xff00) != (addr & 0xff00))) {
                ticks++;
            }

            return addr;
        }

        case CPU_MODE::IMMEDIATE: {
            return pc.w++;
        }

        case INDIRECT: {
            uint8_t l = fetch();
            uint8_t h = fetch();
            uint8_t addrL = read(Utils::MakeAddress(h, l));
            uint8_t addrH = read(Utils::MakeAddress(h, (l + 1) & 0xff));

            return Utils::MakeAddress(addrH, addrL);
        }

        case CPU_MODE::INDIRECT_X: {
            uint8_t base = (fetch() + x) & 0xff;
            uint8_t l = read(base);
            uint8_t h = read((base + 1) & 0xff);

            return Utils::MakeAddress(h, l);
        }

        case CPU_MODE::INDIRECT_Y: case CPU_MODE::INDIRECT_Y_PC: {
            uint8_t base = fetch();
            uint8_t l = read(base);
            uint8_t h = read((base + 1) & 0xff);
            uint16_t before = Utils::MakeAddress(h, l);
            uint16_t addr = (before + y) & 0xffff;

            if ((mode == CPU_MODE::INDIRECT_Y_PC) && ((before & 0xff00) != (addr & 0xff00))) {
                ticks++;
            }

            return addr;
        }

        case CPU_MODE::ZERO_PAGE: {
            return fetch();
        }

        case CPU_MODE::ZERO_PAGE_X: {
            return ((fetch() + x) & 0xff);
        }

        case CPU_MODE::ZERO_PAGE_Y: {
            return ((fetch() + y) & 0xff);
        }

        default: {
            std::printf("CPU :: invalid addressing mode %i", mode);
            std::cin.get();
            exit(1);
        }
    }
}

void CPU::_jump(CPU_MODE mode, int cycles) {
    pc.w = address(mode);
    ticks += cycles;
}

void CPU::_load(CPU_MODE mode, uint8_t &reg, int cycles) {
    reg = read(address(mode));
    checkFlag(CPU_FLAG::Z, reg == 0);
    checkFlag(CPU_FLAG::N, Utils::IsByteNegative(reg));
    ticks += cycles;
}

void CPU::_store(CPU_MODE mode, uint8_t reg, int cycles) {
    write(address(mode), reg);
    ticks += cycles;
}

void CPU::_subroutine(int cycles) {
    uint16_t addr = address(CPU_MODE::ABSOLUTE);

    pc.w--;
    stackPush(pc.h);
    stackPush(pc.l);
    pc.w = addr;
    ticks += cycles;
}

void CPU::_flag(CPU_FLAG flag, bool value, int cycles) {
    checkFlag(flag, value);
    ticks += cycles;
}

void CPU::_branch(CPU_FLAG flag, bool value, int cycles) {
    int16_t n = fetch();

    if (testFlag(flag) == value) {
        ticks++;

        if (Utils::IsByteNegative(n)) {
            n -= 0x100;
        }

        uint8_t before = pc.h;

        pc.w += n;

        if (before != (pc.h)) {
            ticks++;
        }
    }

    ticks += cycles;
}

void CPU::_bit(CPU_MODE mode, int cycles) {
    uint8_t n = read(address(mode));

    checkFlag(CPU_FLAG::Z, (a & n) == 0);
    checkFlag(CPU_FLAG::V, Utils::TestByteBit(n, 6));
    checkFlag(CPU_FLAG::N, Utils::TestByteBit(n, 7));
    ticks += cycles;
}

void CPU::_returnSub(int cycles) {
    pc.l = stackPop();
    pc.h = stackPop();
    pc.w++;
    ticks += cycles;
}

void CPU::_push(uint8_t reg, bool isF, int cycles) {
    stackPush(isF ? (reg | 0x30) : reg);
    ticks += cycles;
}

void CPU::_pull(uint8_t &reg, bool isF, int cycles) {
    reg = stackPop();

    if (isF) {
        clearFlag(CPU_FLAG::B);
        setFlag(CPU_FLAG::P);
    } else {
        checkFlag(CPU_FLAG::Z, reg == 0);
        checkFlag(CPU_FLAG::N, Utils::IsByteNegative(reg));
    }

    ticks += cycles;
}

void CPU::_and(CPU_MODE mode, int cycles) {
    a &= read(address(mode));
    checkFlag(CPU_FLAG::Z, a == 0);
    checkFlag(CPU_FLAG::N, Utils::IsByteNegative(a));
    ticks += cycles;
}

void CPU::_compare(CPU_MODE mode, uint8_t reg, int cycles) {
    uint8_t n = read(address(mode));

    checkFlag(CPU_FLAG::C, reg >= n);
    checkFlag(CPU_FLAG::Z, reg == n);
    checkFlag(CPU_FLAG::N, Utils::IsByteNegative((reg - n) & 0xff));
    ticks += cycles;
}

void CPU::_or(CPU_MODE mode, int cycles) {
    a |= read(address(mode));
    checkFlag(CPU_FLAG::Z, a == 0);
    checkFlag(CPU_FLAG::N, Utils::IsByteNegative(a));
    ticks += cycles;
}

void CPU::_xor(CPU_MODE mode, int cycles) {
    a ^= read(address(mode));
    checkFlag(CPU_FLAG::Z, a == 0);
    checkFlag(CPU_FLAG::N, Utils::IsByteNegative(a));
    ticks += cycles;
}

void CPU::_adc(CPU_MODE mode, int cycles) {
    uint8_t n = read(address(mode));
    uint16_t r = a + n + (testFlag(CPU_FLAG::C) ? 1 : 0);

    checkFlag(CPU_FLAG::C, r > 0xff);
    checkFlag(CPU_FLAG::Z, (r & 0xff) == 0);
    checkFlag(CPU_FLAG::N, Utils::IsByteNegative(r & 0xff));
    r &= 0xff;

    bool nSign = Utils::IsByteNegative((a ^ n) & 0xff);
    bool rSign = Utils::IsByteNegative((a ^ r) & 0xff);

    checkFlag(CPU_FLAG::V, (! nSign) && rSign);
    a = r;
    ticks += cycles;
}

void CPU::_sbc(CPU_MODE mode, int cycles) {
    uint8_t n = read(address(mode));
    uint16_t r = a - n - (testFlag(CPU_FLAG::C) ? 0 : 1);

    checkFlag(CPU_FLAG::C, r < 0x100);
    checkFlag(CPU_FLAG::Z, (r & 0xff) == 0);
    checkFlag(CPU_FLAG::N, Utils::IsByteNegative(r & 0xff));
    r &= 0xff;

    bool nSign = Utils::IsByteNegative((a ^ n) & 0xff);
    bool rSign = Utils::IsByteNegative((a ^ r) & 0xff);

    checkFlag(CPU_FLAG::V, nSign && rSign);
    a = r;
    ticks += cycles;
}

void CPU::_incReg(uint8_t &reg, int cycles) {
    reg++;
    checkFlag(CPU_FLAG::Z, reg == 0);
    checkFlag(CPU_FLAG::N, Utils::IsByteNegative(reg));
    ticks += cycles;
}

void CPU::_decReg(uint8_t &reg, int cycles) {
    reg--;
    checkFlag(CPU_FLAG::Z, reg == 0);
    checkFlag(CPU_FLAG::N, Utils::IsByteNegative(reg));
    ticks += cycles;
}

void CPU::_incMem(CPU_MODE mode, int cycles) {
    uint16_t addr = address(mode);
    uint8_t n = read(addr);

    n++;
    checkFlag(CPU_FLAG::Z, n == 0);
    checkFlag(CPU_FLAG::N, Utils::IsByteNegative(n));
    write(addr, n);
    ticks += cycles;
}

void CPU::_decMem(CPU_MODE mode, int cycles) {
    uint16_t addr = address(mode);
    uint8_t n = read(addr);

    n--;
    checkFlag(CPU_FLAG::Z, n == 0);
    checkFlag(CPU_FLAG::N, Utils::IsByteNegative(n));
    write(addr, n);
    ticks += cycles;
}

void CPU::_transfer(uint8_t src, uint8_t &dest, bool doFlags, int cycles) {
    dest = src;

    if (doFlags) {
        checkFlag(CPU_FLAG::Z, dest == 0);
        checkFlag(CPU_FLAG::N, Utils::IsByteNegative(dest));
    }

    ticks += cycles;
}

void CPU::_returnInt(int cycles) {
    f = stackPop();
    clearFlag(CPU_FLAG::B);
    setFlag(CPU_FLAG::P);
    pc.l = stackPop();
    pc.h = stackPop();
    ticks += cycles;
}

void CPU::_shiftRight(CPU_MODE mode, int cycles) {
    uint16_t addr = ((mode == CPU_MODE::ACCUMULATOR) ? 0 : address(mode));
    uint8_t n = ((mode == CPU_MODE::ACCUMULATOR) ? a : read(addr));
    bool carry = Utils::TestByteBit(n, 0);

    n = ((n >> 1) & 0x7f);
    checkFlag(CPU_FLAG::C, carry);
    checkFlag(CPU_FLAG::Z, n == 0);
    checkFlag(CPU_FLAG::N, Utils::IsByteNegative(n));

    if (mode == CPU_MODE::ACCUMULATOR) {
        a = n;
    } else {
        write(addr, n);
    }

    ticks += cycles;
}

void CPU::_shiftLeft(CPU_MODE mode, int cycles) {
    uint16_t addr = ((mode == CPU_MODE::ACCUMULATOR) ? 0 : address(mode));
    uint8_t n = ((mode == CPU_MODE::ACCUMULATOR) ? a : read(addr));
    bool carry = Utils::TestByteBit(n, 7);

    n = ((n << 1) & 0xfe);
    checkFlag(CPU_FLAG::C, carry);
    checkFlag(CPU_FLAG::Z, n == 0);
    checkFlag(CPU_FLAG::N, Utils::IsByteNegative(n));

    if (mode == CPU_MODE::ACCUMULATOR) {
        a = n;
    } else {
        write(addr, n);
    }

    ticks += cycles;
}

void CPU::_rotateRight(CPU_MODE mode, int cycles) {
    uint16_t addr = ((mode == CPU_MODE::ACCUMULATOR) ? 0 : address(mode));
    uint8_t n = ((mode == CPU_MODE::ACCUMULATOR) ? a : read(addr));
    uint8_t b7 = (testFlag(CPU_FLAG::C) ? 0x80 : 0);
    bool carry = Utils::TestByteBit(n, 0);

    n = ((((n >> 1) & 0x7f) | b7) & 0xff);
    checkFlag(CPU_FLAG::C, carry);
    checkFlag(CPU_FLAG::Z, n == 0);
    checkFlag(CPU_FLAG::N, Utils::IsByteNegative(n));

    if (mode == CPU_MODE::ACCUMULATOR) {
        a = n;
    } else {
        write(addr, n);
    }

    ticks += cycles;
}

void CPU::_rotateLeft(CPU_MODE mode, int cycles) {
    uint16_t addr = ((mode == CPU_MODE::ACCUMULATOR) ? 0 : address(mode));
    uint8_t n = ((mode == CPU_MODE::ACCUMULATOR) ? a : read(addr));
    uint8_t b0 = (testFlag(CPU_FLAG::C) ? 0x1 : 0);
    bool carry = Utils::TestByteBit(n, 7);

    n = ((((n << 1) & 0xfe) | b0) & 0xff);
    checkFlag(CPU_FLAG::C, carry);
    checkFlag(CPU_FLAG::Z, n == 0);
    checkFlag(CPU_FLAG::N, Utils::IsByteNegative(n));

    if (mode == CPU_MODE::ACCUMULATOR) {
        a = n;
    } else {
        write(addr, n);
    }

    ticks += cycles;
}

#if NESTEST_MODE
void CPU::trace(uint8_t opcode) {
    FILE *fp = fopen("/home/lebird/Downloads/output.txt", "at");
    uint8_t op1 = read(pc.w);
    uint8_t op2 = read((pc.w + 1) & 0xffff);
    CPU_OPCODE data;

    try {
        data = opcodesTable.at(opcode);
    } catch (const std::exception &) {
        std::printf("CPU :: data not found for opcode %02X", opcode);
        std::cin.get();
        exit(1);
    }

    // address + opcode
    fprintf(fp, "%04X  %02X ", (pc.w - 1) & 0xffff, opcode);

    // operands + name
    if (data.size == 1) {
        fprintf(fp, "       %s ", data.name);
    } else if (data.size == 2) {
        fprintf(fp, "%02X     %s ", op1, data.name);
    } else if (data.size == 3) {
        fprintf(fp, "%02X %02X  %s ", op1, op2, data.name);
    }

    // operation
    switch (data.mode) {
        case CPU_MODE::ABSOLUTE: {
            uint16_t addr = Utils::MakeAddress(op2, op1);

            if (strcmp(data.name, "JMP") == 0 || strcmp(data.name, "JSR") == 0) {
                fprintf(fp, "$%04X%23s", addr, "");
            } else {
                fprintf(fp, "$%04X = %02X%18s", addr, read(addr), "");
            }
        } break;

        case CPU_MODE::ABSOLUTE_X: case CPU_MODE::ABSOLUTE_X_PC: {
            uint16_t base = Utils::MakeAddress(op2, op1);
            uint16_t addr = (base + x) & 0xffff;

            fprintf(fp, "$%04X,X @ %04X = %02X%9s", base, addr, read(addr), "");
        } break;

        case CPU_MODE::ABSOLUTE_Y: case CPU_MODE::ABSOLUTE_Y_PC: {
            uint16_t base = Utils::MakeAddress(op2, op1);
            uint16_t addr = (base + y) & 0xffff;

            fprintf(fp, "$%04X,Y @ %04X = %02X%9s", base, addr, read(addr), "");
        } break;

        case CPU_MODE::ACCUMULATOR: {
            fprintf(fp, "A%27s", "");
        } break;

        case CPU_MODE::IMMEDIATE: {
            fprintf(fp, "#$%02X%24s", op1, "");
        } break;

        case CPU_MODE::IMPLIED: {
            fprintf(fp, "%28s", "");
        } break;

        case CPU_MODE::INDIRECT: {
            uint16_t base = Utils::MakeAddress(op2, op1);
            uint8_t l = read(base);
            uint8_t h = read((base + 1) & 0xffff);

            fprintf(fp, "($%04X) = %04X%14s", base, Utils::MakeAddress(h, l), "");
        } break;

        case CPU_MODE::INDIRECT_X: {
            uint8_t base = (op1 + x) & 0xff;
            uint8_t l = read(base);
            uint8_t h = read((base + 1) & 0xff);
            uint16_t addr = Utils::MakeAddress(h, l);

            fprintf(fp, "($%02X,X) @ %02X = %04X = %02X%4s", op1, base, addr, read(addr), "");
        } break;

        case CPU_MODE::INDIRECT_Y: case CPU_MODE::INDIRECT_Y_PC: {
            uint8_t l = read(op1);
            uint8_t h = read((op1 + 1) & 0xff);
            uint16_t base = Utils::MakeAddress(h, l);
            uint16_t addr = (base + y) & 0xffff;

            fprintf(fp, "($%02X),Y = %04X @ %04X = %02X  ", op1, base, addr, read(addr));
        } break;

        case CPU_MODE::RELATIVE: {
            fprintf(fp, "$%04X%23s", (pc.w + op1 + 1) & 0xffff, "");
        } break;

        case CPU_MODE::ZERO_PAGE: {
            fprintf(fp, "$%02X = %02X%20s", op1, read(op1), "");
        } break;

        case CPU_MODE::ZERO_PAGE_X: {
            uint8_t addr = (op1 + x) & 0xff;

            fprintf(fp, "$%02X,X @ %02X = %02X%13s", op1, addr, read(addr), "");
        } break;

        case CPU_MODE::ZERO_PAGE_Y: {
            uint8_t addr = (op1 + y) & 0xff;

            fprintf(fp, "$%02X,Y @ %02X = %02X%13s", op1, addr, read(addr), "");
        } break;
    }

    if (totalTicks >= 341) {
        totalTicks -= 341;
    }

    // registers
    fprintf(fp, "A:%02X X:%02X Y:%02X P:%02X SP:%02X CYC:%3i\n", a, x, y, f, sp, totalTicks);
    fclose(fp);
}
#endif

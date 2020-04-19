#include "cpu.h"

CPU::CPU(uint8_t (*readHandler)(uint16_t), void (*writeHandler)(uint16_t, uint8_t)) {
    read = readHandler;
    write = writeHandler;
}

CPU::~CPU() {}

void CPU::reset() {
    if (NESTEST_MODE) {
        pc.w = 0xc000;
    } else {
        pc.l = read(0xfffc);
        pc.h = read(0xfffd);
    }

    a = 0;
    x = 0;
    y = 0;
    sp = 0xfd;
    f = 0x24;
    ticks = 0;
    totalTicks = 0;
}

uint8_t CPU::fetch() {
    return read(pc.w++);
}

int CPU::execute(uint8_t opcode) {
    ticks = 0;

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
        case 0x68: _pull(a, false, 40); break;
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
        case 0xaa: _transfer(x, a, true, 2); break;
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

    totalTicks += ticks;

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

void CPU::_pull(uint8_t reg, bool isF, int cycles) {
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

    checkFlag(CPU_FLAG::C, a >= n);
    checkFlag(CPU_FLAG::Z, a == n);
    checkFlag(CPU_FLAG::N, Utils::IsByteNegative((a - n) & 0xff));
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
    uint16_t addr = ((mode == CPU_MODE::ACCUMULATOR) ? a : address(mode));
    uint8_t n = ((mode == CPU_MODE::ACCUMULATOR) ? 0 : read(addr));
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
    uint16_t addr = ((mode == CPU_MODE::ACCUMULATOR) ? a : address(mode));
    uint8_t n = ((mode == CPU_MODE::ACCUMULATOR) ? 0 : read(addr));
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
    uint16_t addr = ((mode == CPU_MODE::ACCUMULATOR) ? a : address(mode));
    uint8_t n = ((mode == CPU_MODE::ACCUMULATOR) ? 0 : read(addr));
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
    uint16_t addr = ((mode == CPU_MODE::ACCUMULATOR) ? a : address(mode));
    uint8_t n = ((mode == CPU_MODE::ACCUMULATOR) ? 0 : read(addr));
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

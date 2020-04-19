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
    cycles = 0;
    totalCycles = 0;
}

uint8_t CPU::fetch() {
    return read(pc.w++);
}

int CPU::execute(uint8_t opcode) {
    cycles = 0;

    if (! cycles) {
        std::printf("CPU :: cycles not updated for opcode %02X", opcode);
        std::cin.get();
        exit(1);
    }

    return cycles;
}

#include "ppu.h"

PPU::PPU(uint8_t (*readVramHandler)(uint16_t), void (*writeVramHandler)(uint16_t, uint8_t)) {
    readVram = readVramHandler;
    writeVram = writeVramHandler;
    regs.r0.value = 0;
    regs.r1.value = 0;
    regs.r2.value = 0;
    regs.r5.value = 0;
    regs.r6.value = 0;
    fineX = 0;
    ticks = 0;
    scanline = 0;
    firstRead = true;
    pendingNmi = false;
    frameReady = false;
}

PPU::~PPU() {}

bool PPU::wantNmi() {
    bool result = pendingNmi;

    if (pendingNmi) {
        pendingNmi = false;
    }

    return result;
}

bool PPU::readyToRender() {
    bool result = frameReady;

    if (frameReady) {
        frameReady = false;
    }

    return result;
}

uint8_t PPU::readRegister(int reg) {
    switch (reg) {
        case 2: {
            uint8_t r = regs.r2.value;

            regs.r2.vblank = 0;
            firstRead = true;

            return r;
        }

        default: {
            std::printf("PPU :: invalid read to registes %i", reg);
            std::cin.get();
            exit(1);
        }
    }
}

void PPU::writeRegister(int reg, uint8_t value) {
    regs.r2.openBus = value;

    switch (reg) {
        case 0: {
            regs.r0.value = value;
            regs.r5.nameTable = value;
        } break;

        case 1: {
            regs.r1.value = value;
        } break;

        case 3: {
            // TODO: sprites
        } break;

        case 5: {
            if (firstRead) {
                regs.r5.coarseX = (value >> 3);
                fineX = (value & 0x7);
            } else {
                regs.r5.coarseY = (value >> 3);
                regs.r5.fineY = value;
            }

            firstRead = (! firstRead);
        } break;

        case 6: {
            if (firstRead) {
                regs.r5.value &= 0x80ff;
                regs.r5.value |= ((value << 8) & 0x3f00);
            } else {
                regs.r5.value &= 0xff00;
                regs.r5.value |= value;
                regs.r6.value = regs.r5.value;
            }

            firstRead = (! firstRead);
        } break;

        case 7: {
            writeVram(regs.r6.addr, value);
            regs.r6.addr += (regs.r0.increment ? 32 : 1);
        } break;

        default: {
            std::printf("PPU :: invalid write to registes %i", reg);
            std::cin.get();
            exit(1);
        }
    }
}

void PPU::update() {
    // post-render scanline
    if (scanline == 241) {
        frameReady = true;
        regs.r2.vblank = 1;

        if (regs.r0.nmi) {
            pendingNmi = true;
        }
    }

    // last vblank scanline
    else if (scanline == 261) {
        // clear vblank here
        scanline = -1;
    }

    scanline++;
}

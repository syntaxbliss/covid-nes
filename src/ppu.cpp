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
    isOddFrame = false;
    renderAddress = 0;
    pipeline.bkgShiftL = 0;
    pipeline.bkgShiftH = 0;
    pipeline.atrShiftL = 0;
    pipeline.atShiftH = 0;
    pipeline.bkgLatchL = 0;
    pipeline.bkgLatchH = 0;
    pipeline.atrLatchL = false;
    pipeline.atrLatchH = false;
    tileNumber = 0;
    attributeByte = 0;
    delayedBuffer = 0;
}

PPU::~PPU() {}

#if DEBUG_MODE
uint16_t PPU::getBasePT() {
    return (regs.r0.bkgPattern * 0x1000);
}
#endif

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

bool PPU::isRendering() {
    return (regs.r1.bkgVisible || regs.r1.sprVisible);
}

uint8_t PPU::readRegister(int reg) {
    switch (reg) {
        case 2: {
            uint8_t r = regs.r2.value;

            regs.r2.vblank = 0;
            firstRead = true;

            return r;
        }

        case 7: {
            if (regs.r6.addr > 0x3f00) {
                delayedBuffer = regs.r6.addr - 0x1000;

                return readVram(regs.r6.addr);
            } else {
                uint8_t r = delayedBuffer;

                delayedBuffer = readVram(regs.r6.addr);

                return r;
            }

            regs.r6.addr += (regs.r0.increment ? 32 : 1);
        }

        default: {
            std::printf("PPU :: invalid read to register %i", reg);
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
            std::printf("PPU :: invalid write to register %i", reg);
            std::cin.get();
            exit(1);
        }
    }
}

void PPU::update(int cycles) {
    for (int i = 0; i < cycles; i++) {
        switch (scanline) {
            case 0 ... 239: {
                stepScanline<VISIBLE>();
            } break;

            case 240: {
                stepScanline<POST_RENDER>();
            } break;

            case 241: {
                stepScanline<VBLANK>();
            } break;

            case 261: {
                stepScanline<PRE_RENDER>();
            } break;
        }

        ticks++;

        if (ticks > 340) {
            ticks %= 341;
            scanline++;

            if (scanline > 261) {
                scanline = 0;
                isOddFrame = (! isOddFrame);
            }
        }
    }
}

template<PPU_SCANLINE type>
void PPU::stepScanline() {
    if ((type == PPU_SCANLINE::VBLANK) && (ticks == 1)) {
        regs.r2.vblank = 1;

        if (regs.r0.nmi) {
            pendingNmi = true;
        }
    } else if ((type == PPU_SCANLINE::POST_RENDER) && (ticks == 0)) {
        frameReady = true;
    } else if ((type == PPU_SCANLINE::VISIBLE) || (type == PPU_SCANLINE::PRE_RENDER)) {
        switch (ticks) {
            case 2 ... 255: case 322 ... 337: {
                // renderPixel();

                switch (ticks % 8) {
                    case 1: {
                        renderAddress = ntAddress();
                        updatePipeline();
                    } break;

                    case 2: {
                        tileNumber = readVram(renderAddress);
                    } break;

                    case 3: {
                        renderAddress = atrAddress();
                    } break;

                    case 4: {
                        attributeByte = readVram(renderAddress);

                        if (regs.r6.coarseY & 2) {
                            attributeByte >>= 4;
                        }

                        if (regs.r6.coarseX & 2) {
                            attributeByte >>= 2;
                        }
                    } break;

                    case 5: {
                        renderAddress = ptAddress();
                    } break;

                    case 6: {
                        pipeline.bkgLatchL = readVram(renderAddress);
                    } break;

                    case 7: {
                        renderAddress += 8;
                    } break;

                    case 0: {
                        pipeline.bkgLatchH = readVram(renderAddress);
                        horizontalScroll();
                    } break;
                } break;
            } break;

            case 256: {
                // renderPixel();
                pipeline.bkgLatchH = readVram(renderAddress);
                verticalScroll();
            } break;

            case 257: {
                // rendrPixel();
                updatePipeline();
                horizontalReload();
            } break;

            case 280 ... 304: {
                if (type == PPU_SCANLINE::PRE_RENDER) {
                    verticalReload();
                }
            } break;

            case 1: {
                renderAddress = ntAddress();

                if (type == PPU_SCANLINE::PRE_RENDER) {
                    regs.r2.vblank = 0;
                }
            } break;

            case 321: case 339: {
                renderAddress = ntAddress();
            } break;

            case 338: {
                tileNumber = readVram(renderAddress);
            } break;

            case 340: {
                tileNumber = readVram(renderAddress);

                if (type == PPU_SCANLINE::PRE_RENDER && isRendering() && isOddFrame) {
                    ticks++;
                }
            } break;
        }
    }
}

void PPU::updatePipeline() {
    pipeline.bkgShiftL = ((pipeline.bkgShiftL & 0xff00) | pipeline.bkgLatchL);
    pipeline.bkgShiftH = ((pipeline.bkgShiftH & 0xff00) | pipeline.bkgLatchH);
    pipeline.atrLatchL = (attributeByte & 1);
    pipeline.atrLatchH = (attributeByte & 2);
}

uint16_t PPU::ntAddress() {
    return (0x2000 + (regs.r6.addr & 0xfff));
}

uint16_t PPU::atrAddress() {
    return (
        0x23C0 |
        (regs.r6.nameTable << 10) |
        ((regs.r6.coarseY >> 2) << 3) |
        (regs.r6.coarseX >> 2)
    );
}

uint16_t PPU::ptAddress() {
    return (regs.r0.bkgPattern * 0x1000) + (tileNumber * 16) + regs.r6.fineY;
}

void PPU::horizontalScroll() {
    if (isRendering()) {
        if (regs.r6.coarseX == 31) {
            regs.r6.value ^= 0x41f;
        } else {
            regs.r6.coarseX++;
        }
    }
}

void PPU::verticalScroll() {
    if (isRendering()) {
        if (regs.r6.fineY < 7) {
            regs.r6.fineY++;
        } else {
            regs.r6.fineY = 0;

            if (regs.r6.coarseY == 31) {
                regs.r6.coarseY = 0;
            } else if (regs.r6.coarseY == 29) {
                regs.r6.coarseY = 0;
                regs.r6.nameTable ^= 0x2;
            } else {
                regs.r6.coarseY++;
            }
        }
    }
}

void PPU::horizontalReload() {
    if (isRendering()) {
        regs.r6.value &= (~0x041f);
        regs.r6.value |= (regs.r5.value & 0x041f);
    }
}

void PPU::verticalReload() {
    if (isRendering()) {
        regs.r6.value &= (~0x7be0);
        regs.r6.value |= (regs.r5.value & 0x7be0);
    }
}

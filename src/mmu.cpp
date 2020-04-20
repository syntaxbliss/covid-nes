#include "mmu.h"

MMU::MMU() {
    rom = (uint8_t*)malloc(0x10000);
    vram = (uint8_t*)malloc(0x4000);
    sram = (uint8_t*)malloc(0x100);
    memset(rom, 0, 0x10000);
    memset(vram, 0, 0x4000);
    memset(sram, 0, 0x100);
    ntMirroring = NT_MIRRORING::HORIZONTAL;
}

MMU::~MMU() {
    free(rom);
    free(vram);
    free(sram);
}

void MMU::allocate(uint8_t *cartridge) {
    int romBanks = cartridge[4];

    if (romBanks == 1) {
        memcpy(&rom[0x8000], &cartridge[16], 0x4000);
        memcpy(&rom[0xc000], &cartridge[16], 0x4000);
    } else if (romBanks == 2) {
        memcpy(&rom[0x8000], &cartridge[16], 0x8000);
    } else {
        std::cout << "MMU :: too many PRG-ROM banks" << std::endl;
        std::cin.get();
        exit(1);
    }

    memcpy(vram, &cartridge[16 + 0x4000 * romBanks], 0x2000);

    ntMirroring = (Utils::TestByteBit(cartridge[6], 0) ? NT_MIRRORING::VERTICAL : NT_MIRRORING::HORIZONTAL);
}

uint8_t MMU::readRom(uint16_t address) {
    return rom[address];
}

void MMU::writeRom(uint16_t address, uint8_t value) {
    rom[address] = value;
}

uint8_t MMU::readVram(uint16_t address) {
    uint16_t addr = (address & 0x3fff);

    // pattern tables
    if (addr < 0x2000) {
        return vram[addr];
    }

    // name tables
    else if ((addr >= 0x2000) && (addr < 0x3f00)) {
        if (addr >= 0x3000) {
            addr -= 0x1000;
        }

        return vram[ntAddress(addr)];
    }

    // palettes
    else {
        // TODO: palettes
        return vram[addr];
    }
}

void MMU::writeVram(uint16_t address, uint8_t value) {
    uint16_t addr = (address & 0x3fff);

    // pattern tables
    if (addr < 0x2000) {
        // TODO: mappers
        std::cout << "MMU :: trying to write to a pattern table" << std::endl;
        std::cin.get();
        exit(1);
    }

    // name tables
    else if ((addr >= 0x2000) && (addr < 0x3f00)) {
        if (addr >= 0x3000) {
            addr -= 0x1000;
        }

        vram[ntAddress(addr)] = value;
    }

    // palettes
    else {
        // TODO: palettes
        vram[addr] = value;
    }
}

uint16_t MMU::ntAddress(uint16_t address) {
    uint16_t r = address;

    if (ntMirroring == NT_MIRRORING::HORIZONTAL) {
        switch (address & 0x2c00) {
            case 0x2400: case 0x2c00: {
                r = (address - 0x400);
            } break;
        }
    } else {
        switch (address & 0x2c00) {
            case 0x2800: case 0x2c00: {
                r = (address - 0x800);
            } break;
        }
    }

    return r;
}

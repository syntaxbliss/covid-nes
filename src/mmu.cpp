#include "mmu.h"

MMU::MMU() {
    rom = (uint8_t*)malloc(0x10000);
    vram = (uint8_t*)malloc(0x4000);
    sram = (uint8_t*)malloc(0x100);
    memset(rom, 0, 0x10000);
    memset(vram, 0, 0x4000);
    memset(sram, 0, 0x100);
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
}

#include "nes.h"

uint8_t BridgeReadRom(uint16_t address) {
    return NES::GetInstance()->readRom(address);
}

void BridgeWriteRom(uint16_t address, uint8_t value) {
    NES::GetInstance()->writeRom(address, value);
}

NES* NES::Instance = nullptr;

NES::NES() {
    mmu = new MMU();
    cpu = new CPU(&BridgeReadRom, &BridgeWriteRom);
    running = false;
}

NES::~NES() {
    delete mmu;
    delete cpu;
}

NES* NES::GetInstance() {
    if (! Instance) {
        Instance = new NES();
    }

    return Instance;
}

void NES::loadFile(const char *path) {
    FILE *fp = fopen(path, "rb");

    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    uint8_t* cartridge = (uint8_t*)malloc(size);
    fseek(fp, 0, SEEK_SET);
    fread(cartridge, 1, size, fp);
    fclose(fp);
    mmu->allocate(cartridge);
    free(cartridge);
}

void NES::start() {
    cpu->reset();

    running = true;

    for (int i = 0; i < 5100; i++) {
        cpu->execute(cpu->fetch());
    }
}

uint8_t NES::readRom(uint16_t address) {
    return mmu->readRom(address);
}

void NES::writeRom(uint16_t address, uint8_t value) {
    mmu->writeRom(address, value);
}

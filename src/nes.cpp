#include "nes.h"

NES* NES::Instance = nullptr;

NES::NES() {
    mmu = new MMU();
}

NES::~NES() {
    delete mmu;
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

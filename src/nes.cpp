#include "nes.h"

uint8_t BridgeReadRom(uint16_t address) {
    return NES::GetInstance()->readRom(address);
}

void BridgeWriteRom(uint16_t address, uint8_t value) {
    NES::GetInstance()->writeRom(address, value);
}

uint8_t BridgeReadVram(uint16_t address) {
    return NES::GetInstance()->readVram(address);
}

void BridgeWriteVram(uint16_t address, uint8_t value) {
    NES::GetInstance()->writeVram(address, value);
}

NES* NES::Instance = nullptr;

NES::NES() {
    screenBuffer = (int*)malloc(sizeof(int) * 256 * 240);
    mmu = new MMU();
    cpu = new CPU(&BridgeReadRom, &BridgeWriteRom);
    ppu = new PPU(&BridgeReadVram, &BridgeWriteVram, screenBuffer);
    running = false;
    screen = (EMULATOR_WINDOW*)malloc(sizeof(EMULATOR_WINDOW));

    #if DEBUG_MODE
    debugger = new Debugger(mmu, ppu);
    #endif
}

NES::~NES() {
    delete mmu;
    delete cpu;
    delete ppu;
    free(screen);
    free(screenBuffer);
    teardownGraphics();
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
    launchGraphics();
    cpu->reset();

    running = true;

    while (running) {
        int ticks = cpu->execute(cpu->fetch());

        ppu->update(ticks * 3);
        handleInterrupts();

        if (ppu->readyToRender()) {
            drawScreen();

            #if DEBUG_MODE
            debugger->drawPatterns();
            debugger->drawNameTables();
            #endif
        }
    }
}

void NES::launchGraphics() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::printf("NES :: could not intialize graphics [%s]", SDL_GetError());
        std::cin.get();
        exit(1);
    }

    #if DEBUG_MODE
    debugger->launchGraphics();
    #endif

    Utils::CreateEmulatorWindow(screen, "COVID-NES", 256, 240, 500, 0);
}

void NES::teardownGraphics() {
    #if DEBUG_MODE
    debugger->teardownGraphics();
    #endif

    SDL_Quit();
}

void NES::handleInterrupts() {
    if (ppu->wantNmi()) {
        cpu->nmi();
    }
}

void NES::drawScreen() {
    int *index = screenBuffer;

    SDL_RenderPresent(screen->renderer);
    SDL_SetRenderTarget(screen->renderer, screen->display);

    for (int y = 0; y < 240; y++) {
        for (int x = 0; x < 256; x++) {
            uint32_t color = NES_PALETTE[*index];

            SDL_SetRenderDrawColor(
                screen->renderer,
                (color >> 16) & 0xff,
                (color >> 8) & 0xff,
                color & 0xff,
                255
            );
            SDL_RenderDrawPoint(screen->renderer, x, y);
            index++;
        }
    }

    SDL_SetRenderTarget(screen->renderer, NULL);
    SDL_RenderCopy(screen->renderer, screen->display, NULL, NULL);
    SDL_RenderPresent(screen->renderer);
}

uint8_t NES::readRom(uint16_t address) {
    // ppu regs
    if ((address >= 0x2000) && (address < 0x4000)) {
        return ppu->readRegister(address & 0x7);
    }

    else {
        return mmu->readRom(address);
    }
}

void NES::writeRom(uint16_t address, uint8_t value) {
    // ppu regs
    if ((address >= 0x2000) && (address < 0x4000)) {
        ppu->writeRegister(address & 0x7, value);
    }

    else {
        mmu->writeRom(address, value);
    }
}

uint8_t NES::readVram(uint16_t address) {
    return mmu->readVram(address);
}

void NES::writeVram(uint16_t address, uint8_t value) {
    mmu->writeVram(address, value);
}

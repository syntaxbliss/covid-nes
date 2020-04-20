#include "debugger.h"

Debugger::Debugger(MMU *mmuInstance, PPU* ppuInstance) {
    mmu = mmuInstance;
    ppu = ppuInstance;
    patterns = (EMULATOR_WINDOW*)malloc(sizeof(EMULATOR_WINDOW));
    nameTables = (EMULATOR_WINDOW*)malloc(sizeof(EMULATOR_WINDOW));
}

Debugger::~Debugger() {}

void Debugger::launchGraphics() {
    Utils::CreateEmulatorWindow(patterns, "Pattern Tables", 128, 256, 500, 300);
    Utils::CreateEmulatorWindow(nameTables, "Name Tables", 512, 480, 650, 300);
}

void Debugger::teardownGraphics() {
    SDL_DestroyRenderer(patterns->renderer);
    SDL_DestroyWindow(patterns->window);
    SDL_DestroyRenderer(nameTables->renderer);
    SDL_DestroyWindow(nameTables->window);
}

void Debugger::drawPatterns() {
    SDL_RenderPresent(patterns->renderer);
    SDL_SetRenderTarget(patterns->renderer, patterns->display);

    for (int pt = 0; pt < 2; pt++) {
        for (int tile = 0; tile < 256; tile++) {
            for (int line = 0; line < 8; line++) {
                uint16_t base = pt * 0x1000 + tile * 16 + line;
                uint8_t l = mmu->readVram(base);
                uint8_t h = mmu->readVram(base + 8);

                for (int  pixel = 7; pixel >= 0; pixel--) {
                    int cl = (l >> pixel) & 0x1;
                    int ch = (h >> pixel) & 0x1;
                    int color = DEBUGGER_PALETTE[((ch << 1) | cl) & 0x3];

                    int x = (tile % 16) * 8 + (7 ^ pixel);
                    int y = floor(tile / 16) * 8 + line + pt * 128;

                    SDL_SetRenderDrawColor(
                        patterns->renderer,
                        (color >> 16) & 0xff,
                        (color >> 8) & 0xff,
                        color & 0xff,
                        255
                    );
                    SDL_RenderDrawPoint(patterns->renderer, x, y);
                }
            }
        }
    }

    SDL_SetRenderTarget(patterns->renderer, NULL);
    SDL_RenderCopy(patterns->renderer, patterns->display, NULL, NULL);
    SDL_RenderPresent(patterns->renderer);
}

void Debugger::drawNameTables() {
    uint16_t pt = ppu->getBasePT();

    SDL_RenderPresent(nameTables->renderer);
    SDL_SetRenderTarget(nameTables->renderer, nameTables->display);

    for (int nt = 0; nt < 4; nt++) {
        for (int tile = 0; tile < 960; tile++) {
            uint16_t tileNumber = mmu->readVram(0x2000 + 0x400 * nt + tile);

            for (int line = 0; line < 8; line++) {
                uint16_t address = pt + tileNumber * 16 + line;
                uint8_t l = mmu->readVram(address);
                uint8_t h = mmu->readVram(address + 8);

                for (int pixel = 7; pixel >= 0; pixel--) {
                    int cl = (l >> pixel) & 0x1;
                    int ch = (h >> pixel) & 0x1;
                    int color = DEBUGGER_PALETTE[((ch << 1) | cl) & 0x3];

                    int x = (tile % 32) * 8 + (7 ^ pixel) + (nt % 2) * 256;
                    int y = floor(tile / 32) * 8 + line + floor(nt / 2) * 240;

                    SDL_SetRenderDrawColor(
                        nameTables->renderer,
                        (color >> 16) & 0xff,
                        (color >> 8) & 0xff,
                        color & 0xff,
                        255
                    );
                    SDL_RenderDrawPoint(nameTables->renderer, x, y);
                }
            }
        }
    };

    SDL_SetRenderTarget(nameTables->renderer, NULL);
    SDL_RenderCopy(nameTables->renderer, nameTables->display, NULL, NULL);
    SDL_RenderPresent(nameTables->renderer);
}

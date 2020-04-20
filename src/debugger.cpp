#include "debugger.h"

Debugger::Debugger(MMU *memory) {
    mmu = memory;
}

Debugger::~Debugger() {}

void Debugger::launchGraphics() {
    patterns.width = 128;
    patterns.height = 256;
    patterns.window = SDL_CreateWindow(
        "Patterns",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        patterns.width,
        patterns.height,
        SDL_WINDOW_SHOWN
    );
    patterns.renderer = SDL_CreateRenderer(patterns.window, -1, SDL_RENDERER_SOFTWARE);
    patterns.display = SDL_CreateTexture(
        patterns.renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        patterns.width,
        patterns.height
    );
    SDL_SetRenderDrawColor(patterns.renderer, 0, 0, 0, 0xff);
    SDL_RenderClear(patterns.renderer);
    SDL_RenderPresent(patterns.renderer);
}

void Debugger::teardownGraphics() {
    SDL_DestroyRenderer(patterns.renderer);
    SDL_DestroyWindow(patterns.window);
}

void Debugger::drawPatterns() {
    SDL_RenderPresent(patterns.renderer);
    SDL_SetRenderTarget(patterns.renderer, patterns.display);

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
                        patterns.renderer,
                        (color >> 16) & 0xff,
                        (color >> 8) & 0xff,
                        color & 0xff,
                        255
                    );
                    SDL_RenderDrawPoint(patterns.renderer, x, y);
                }
            }
        }
    }

    SDL_SetRenderTarget(patterns.renderer, NULL);
    SDL_RenderCopy(patterns.renderer, patterns.display, NULL, NULL);
    SDL_RenderPresent(patterns.renderer);
}

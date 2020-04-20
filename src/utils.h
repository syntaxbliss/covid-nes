#pragma once
#ifndef UTILS_H
#define UTILS_H

class Utils {
    public:
        static uint16_t MakeAddress(uint8_t h, uint8_t l) {
            return (((h << 8) | l) & 0xffff);
        }

        static bool TestByteBit(uint8_t value, int bit) {
            uint8_t mask = (1 << bit);

            return ((value & mask) == mask);
        }

        static bool IsByteNegative(uint8_t value) {
            return TestByteBit(value, 7);
        }

        static void CreateEmulatorWindow(
            EMULATOR_WINDOW* w,
            const char *title,
            int width,
            int height,
            int x,
            int y
        ) {
            w->width = width;
            w->height = height;
            w->window = SDL_CreateWindow(title, x, y, w->width, w->height, SDL_WINDOW_SHOWN);
            w->renderer = SDL_CreateRenderer(w->window, -1, SDL_RENDERER_SOFTWARE);
            w->display = SDL_CreateTexture(
                w->renderer,
                SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET,
                w->width,
                w->height
            );
            SDL_SetRenderDrawColor(w->renderer, 0, 0, 0, 0xff);
            SDL_RenderClear(w->renderer);
            SDL_RenderPresent(w->renderer);
        }
};

#endif

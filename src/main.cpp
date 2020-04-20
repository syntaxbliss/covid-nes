#include <iostream>
#include "nes.h"

int main() {
    NES* nes = NES::GetInstance();
    // nes->loadFile("/home/lebird/Development/covid-nes/roms/balloon.nes");
    // nes->loadFile("/home/lebird/Development/covid-nes/roms/kong.nes");
    // nes->loadFile("/home/lebird/Development/covid-nes/roms/kong3.nes");
    // nes->loadFile("/home/lebird/Development/covid-nes/roms/kong_classics.nes");
    // nes->loadFile("/home/lebird/Development/covid-nes/roms/nestest.nes");
    nes->loadFile("/home/lebird/Development/covid-nes/roms/pacman.nes");
    // nes->loadFile("/home/lebird/Development/covid-nes/roms/nestest.nes");
    nes->start();

    delete nes;
    std::cout << "Exit..." << std::endl;
    std::cin.get();

    return 0;
}

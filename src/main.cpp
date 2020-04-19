#include <iostream>
#include "nes.h"

int main() {
    NES* nes = NES::GetInstance();
    nes->loadFile("/home/lebird/Development/covid-nes/roms/nestest.nes");

    delete nes;
    std::cout << "Exit..." << std::endl;
    std::cin.get();

    return 0;
}

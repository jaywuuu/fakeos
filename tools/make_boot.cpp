#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

#include "tools_config.h"

#define BOOT_MAGIC_NUMBER 0xAA55

void printHelp() {
    std::cout << "make_boot version: " << MAKE_BOOT_VERSION_MAJOR << "."
              << MAKE_BOOT_VERSION_MINOR << std::endl;
    std::cout << "Usage: make_boot DISK_IMG\n";
    std::cout << "Writes 512 MB boot sector to DISK_IMG\n";
}

struct MBR {
    uint8_t filler[510];
    uint16_t magicNumber;
};

void setupMBR(MBR& mbr) {
    memset(&mbr, 0, sizeof(MBR));
    mbr.magicNumber = BOOT_MAGIC_NUMBER;
}

void writeBoot(std::fstream& img) {
    MBR mbr = {0};
    setupMBR(mbr);
    img.write(reinterpret_cast<const char*>(&mbr), sizeof(MBR));
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        printHelp();
        return 0;
    }

    std::string imgFilePath = argv[1];

    std::fstream imgFile;
    imgFile.open(imgFilePath, std::ios::in | std::ios::out | std::ios::binary);

    if (!imgFile.is_open())
        throw std::runtime_error("Could not open file: " + imgFilePath);

    writeBoot(imgFile);

    imgFile.flush();
    imgFile.close();

    return 0;
}
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
    std::cout << "Usage: make_boot [OPTIONS] DISK_IMG\n";
    std::cout << "Writes 512 MB boot sector to DISK_IMG\n";
    std::cout << "[OPTIONS]\n";
    std::cout << "-b bootloader     Writes bootloader.\n";
}

struct Options {
    bool writeBootloader;
    std::string bootloaderPath;
    std::string imgFilePath;
};

Options processCommandLineArgs(int argc, char* argv[]) {
    Options cmdLineOptions = {};
    cmdLineOptions.imgFilePath = "";

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-b") == 0) {
            if (i+1 < argc) {
                cmdLineOptions.writeBootloader = true;
                cmdLineOptions.bootloaderPath = argv[++i];
            }
        } else if (strcmp(argv[i], "-h") == 0) {
            printHelp();
        } else {
            cmdLineOptions.imgFilePath = argv[i];
        }
    }

    return cmdLineOptions;
}

#pragma pack(1)
struct MBR {
    uint8_t bootLoader[446];
    uint32_t partitionEntry0[4];
    uint32_t partitionEntry1[4];
    uint32_t partitionEntry2[4];
    uint32_t partitionEntry3[4];
    uint16_t magicNumber;
};

void setupMBR(MBR& mbr) {
    memset(&mbr, 0, sizeof(MBR));
    mbr.magicNumber = BOOT_MAGIC_NUMBER;
}

void writeBoot(Options& opts, std::fstream& img) {
    MBR mbr = {0};
    setupMBR(mbr);

    if (opts.writeBootloader) {
        std::ifstream bootloader;
        bootloader.open(opts.bootloaderPath, std::ios::in | std::ios::binary);
        if (!bootloader.is_open())
            throw std::runtime_error("Could not open bootloader file: " + opts.bootloaderPath);
        
        size_t maxBootloaderSize = sizeof(mbr.bootLoader);
        bootloader.seekg(0, bootloader.end);
        size_t bootloaderSize = bootloader.tellg();

        if (bootloaderSize <= maxBootloaderSize) {
            bootloader.seekg(0, bootloader.beg);
            bootloader.read(reinterpret_cast<char*>(mbr.bootLoader), bootloaderSize);
        }

        bootloader.close();
    }

    img.write(reinterpret_cast<const char*>(&mbr), sizeof(MBR));
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        printHelp();
        return 0;
    }

    Options cmdLineOptions = processCommandLineArgs(argc, argv);

    if (cmdLineOptions.imgFilePath.length() == 0) {
        printHelp();
        return 0;
    }

    std::string imgFilePath = cmdLineOptions.imgFilePath;

    std::fstream imgFile;
    imgFile.open(imgFilePath, std::ios::in | std::ios::out | std::ios::binary);

    if (!imgFile.is_open())
        throw std::runtime_error("Could not open file: " + imgFilePath);

    writeBoot(cmdLineOptions, imgFile);

    imgFile.flush();
    imgFile.close();

    return 0;
}
#pragma once
#include "internal/BlockData.h"
#include <unordered_map>

class ASCIIBlockDictionary {
public:
    // Singleton Zugriff
    static ASCIIBlockDictionary& Instance();

    BlockData getBlockData(char c);

    void setAllColorsRedExceptPlayer();

private:
    ASCIIBlockDictionary();  // Konstruktor private
    ASCIIBlockDictionary(const ASCIIBlockDictionary&) = delete;            // Kopieren verbieten
    ASCIIBlockDictionary& operator=(const ASCIIBlockDictionary&) = delete; // Zuweisung verbieten

    std::unordered_map<std::string, BlockData> blocks;
    int lavaVar = 0;
};

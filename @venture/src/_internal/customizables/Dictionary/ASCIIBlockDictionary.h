#pragma once
#include "_internal/BlockData.h"
#include <unordered_map>
#include <Flux.h>

class ASCIIBlockDictionary {
public:
    // Singleton Zugriff
    static ASCIIBlockDictionary& Instance();

    BlockData getBlockData(char c);

    void setAllColorsRedExceptPlayerAndRootPathway();

    void SetBlockPattern(const std::string& name, const std::vector<char>& newPattern);

private:
    ASCIIBlockDictionary();  // Konstruktor private
    ASCIIBlockDictionary(const ASCIIBlockDictionary&) = delete;            // Kopieren verbieten
    ASCIIBlockDictionary& operator=(const ASCIIBlockDictionary&) = delete; // Zuweisung verbieten

    std::unordered_map<std::string, BlockData> blocks;
    int lavaVar = 0;
};

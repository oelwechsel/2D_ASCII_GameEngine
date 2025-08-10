#pragma once
#include "_internal/BlockData.h"
#include <unordered_map>
#include <Flux.h>

class ASCIIBlockDictionary {
public:
    static ASCIIBlockDictionary& Instance();

    BlockData getBlockData(char _c);

    void setAllColorsRedExceptPlayerAndRootPathway();

    void SetBlockPattern(const std::string& _name, const std::vector<char>& _newPattern);

private:
    ASCIIBlockDictionary();  
    ASCIIBlockDictionary(const ASCIIBlockDictionary&) = delete;         
    ASCIIBlockDictionary& operator=(const ASCIIBlockDictionary&) = delete; 

    std::unordered_map<std::string, BlockData> m_Blocks;
    int m_LavaVar = 0;
};

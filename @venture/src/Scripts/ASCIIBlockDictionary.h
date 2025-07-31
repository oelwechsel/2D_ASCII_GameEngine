#pragma once
#include "BlockData.h"

class ASCIIBlockDictionary {
public:
    ASCIIBlockDictionary();

    BlockData getBlockData(char c);

private:
    std::unordered_map<std::string, BlockData> blocks;
    int lavaVar = 0;
};


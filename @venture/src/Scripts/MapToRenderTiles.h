#pragma once

#include <Flux.h>
#include "ASCIIBlockDictionary.h"

std::vector<Flux::RenderTile> CreateRenderTiles(const std::vector<std::string>& map,
    ASCIIBlockDictionary& dictionary);

//Could be transferred to ASCIIBlockDictionary

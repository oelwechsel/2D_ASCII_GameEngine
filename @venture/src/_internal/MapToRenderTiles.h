#pragma once
#include <Flux.h>
#include "customizables/Dictionary/ASCIIBlockDictionary.h"

inline std::vector<Flux::RenderTile> CreateRenderTiles(
    const std::vector<std::string>& map,
    ASCIIBlockDictionary& dictionary,
    const std::vector<Flux::Entity>& entities)
{
    std::vector<Flux::RenderTile> tiles;

    int height = (int)map.size();
    if (height == 0) return tiles;
    int width = (int)map[0].size();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Flux::Entity* entity = nullptr;
            for (const Flux::Entity& e : entities) {
                if (e.m_xPos == x && e.m_yPos == y) {
                    entity = &e;
                    break;
                }
            }

            BlockData blockData;

            if (entity) {
                blockData = dictionary.getBlockData(entity->m_AsciiName);
            }
            else {
                char mapChar = map[y][x];
                blockData = dictionary.getBlockData(mapChar);
            }

            for (size_t layer = 0; layer < blockData.chars.size(); ++layer) {
                Flux::RenderTile tile;
                tile.x = x;
                tile.y = y;
                tile.layer = static_cast<int>(layer);
                tile.tileChar = blockData.chars[layer];
                RGBA color = GetRGBAFromBlockColor(blockData.color, 1.0f);
                tile.r = color.r;
                tile.g = color.g;
                tile.b = color.b;
                tile.a = color.a;
                tiles.push_back(tile);
            }
        }
    }

    return tiles;
}

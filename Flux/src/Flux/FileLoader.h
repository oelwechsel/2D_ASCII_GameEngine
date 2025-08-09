#pragma once

#include "Flux/Core.h"
#include "Flux/Log.h"
#include "Flux/Entity.h"

using TextureID = unsigned int;

namespace Flux
{
    class FLUX_API FileLoader
    {
    public:

        static unsigned int LoadTextureFromFile(const std::string& filename, int desiredChannels = 4);

        static std::vector<std::string> LoadTextFile(const std::string& filename, bool* success);

        static std::vector<LayeredFrame> LoadAsciiFrames(const std::string& filename);

        static TextureID LoadTileset(const std::string& filename, int tileSize);
    };
}

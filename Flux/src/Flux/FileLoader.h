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
        static unsigned int LoadTextureFromFile(const std::string& _filename, int _desiredChannels = 4);

        static std::vector<std::string> LoadTextFile(const std::string& _filename, bool* _success);

        static std::vector<LayeredFrame> LoadAsciiFrames(const std::string& _filename);

        static TextureID LoadTileset(const std::string& _filename, int _tileSize);
    };
}

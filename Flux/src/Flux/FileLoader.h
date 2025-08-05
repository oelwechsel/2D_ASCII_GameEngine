#pragma once

#include "Flux/Core.h"
#include "Flux/Log.h"

using TextureID = unsigned int;

namespace Flux
{
    struct FLUX_API LayeredFrame {
        std::vector<std::string> layers;
    };


    class FLUX_API FileLoader
    {
    public:

        static unsigned char* LoadImageFromFile(const std::string& filename, int* width, int* height, int* channels, int desiredChannels = 0);

        static std::vector<std::string> LoadTextFile(const std::string& filename, bool* success);

        static void FreeImage(unsigned char* data);

        static std::vector<LayeredFrame> LoadAsciiFrames(const std::string& filename, bool* success);

        static TextureID LoadTileset(const std::string& filename, int tileSize);
    };
}

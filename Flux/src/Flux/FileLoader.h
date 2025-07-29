#pragma once

#include "Flux/Core.h"
#include "Flux/Log.h"

namespace Flux
{
    class FLUX_API FileLoader
    {
    public:

        static unsigned char* LoadImageFromFile(const std::string& filename, int* width, int* height, int* channels, int desiredChannels = 0);

        static void FreeImage(unsigned char* data);
    };
}

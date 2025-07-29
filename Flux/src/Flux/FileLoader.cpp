#include "fxpch.h"
#include "Flux/FileLoader.h"


// Implementierung von stb_image
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Flux
{
    namespace fs = std::filesystem;

    static std::string FindFileInRes(const std::string& filename)
    {
        const fs::path baseDir = fs::current_path() / "src" / "res";

        for (auto& p : fs::recursive_directory_iterator(baseDir))
        {
            if (!p.is_regular_file()) continue;

            if (p.path().filename() == filename)
                return p.path().string();
        }

        return ""; 
    }


    unsigned char* FileLoader::LoadImageFromFile(const std::string& filename, int* width, int* height, int* channels, int desiredChannels)
    {
        std::string resolvedPath = filename;

        if (!fs::exists(filename))
        {
            resolvedPath = FindFileInRes(filename);

            if (resolvedPath.empty())
            {
                FX_CORE_WARN("File not found: {}", filename);
                return nullptr;
            }
        }

        FX_CORE_INFO("Loading Image: {}", resolvedPath);
        return stbi_load(resolvedPath.c_str(), width, height, channels, desiredChannels); 
    }


    void FileLoader::FreeImage(unsigned char* data)
    {
        stbi_image_free(data);
    }
}

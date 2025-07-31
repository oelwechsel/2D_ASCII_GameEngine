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

    std::vector<std::string> FileLoader::LoadTextFileLines(const std::string& filename, bool* success)
    {
        std::string resolvedPath = filename;

        if (!fs::exists(filename))
        {
            resolvedPath = FindFileInRes(filename);

            if (resolvedPath.empty())
            {
                FX_CORE_WARN("Text file not found: {}", filename);
                if (success) *success = false;
                return {};
            }
        }

        std::ifstream file(resolvedPath);
        if (!file.is_open())
        {
            FX_CORE_WARN("Failed to open text file: {}", resolvedPath);
            if (success) *success = false;
            return {};
        }

        FX_CORE_INFO("Loading Text File: {}", resolvedPath);

        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line))
        {
            lines.push_back(line);
        }

        if (success) *success = true;
        return lines;
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

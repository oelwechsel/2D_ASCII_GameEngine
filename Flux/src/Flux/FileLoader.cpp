#include "fxpch.h"
#include "Flux/FileLoader.h"


// Implementierung von stb_image
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "GL/glew.h"


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

    std::vector<std::string> FileLoader::LoadTextFile(const std::string& filename, bool* success)
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

    unsigned int FileLoader::LoadTextureFromFile(const std::string& filename, int desiredChannels)
    {
        std::string resolvedPath = filename;

        if (!fs::exists(filename))
        {
            resolvedPath = FindFileInRes(filename);
            if (resolvedPath.empty())
            {
                FX_CORE_WARN("File not found: {}", filename);
                return 0;
            }
        }

        int width, height, channels;
        unsigned char* data = stbi_load(resolvedPath.c_str(), &width, &height, &channels, desiredChannels);
        if (!data)
        {
            std::cerr << "Failed to load image: " << resolvedPath << std::endl;
            return 0;
        }

        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
            desiredChannels == 4 ? GL_RGBA : GL_RGB,
            GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);

        return textureID;
    }

    std::vector<LayeredFrame> FileLoader::LoadAsciiFrames(const std::string& filename)
    {
        std::string resolvedPath = filename;

        if (!std::filesystem::exists(filename))
        {
            resolvedPath = FindFileInRes(filename);
            if (resolvedPath.empty())
            {
                FX_CORE_WARN("AsciiFrame file not found: {}", filename);
                return {};
            }
        }

        std::ifstream file(resolvedPath);
        if (!file.is_open())
        {
            FX_CORE_WARN("Failed to open AsciiFrame file: {}", resolvedPath);
            return {};
        }

        FX_CORE_INFO("Loading AsciiFrames from: {}", resolvedPath);

        using Frame = std::vector<std::string>;

        std::vector<LayeredFrame> resultFrames;
        Frame currentFrame;
        std::ostringstream currentLayerStream;

        std::string line;
        while (std::getline(file, line))
        {
            if (!line.empty() && line.back() == '\r') line.pop_back();

            if (line == "---FRAME---")
            {
                if (!currentLayerStream.str().empty())
                {
                    currentFrame.push_back(currentLayerStream.str());
                    currentLayerStream.str("");
                    currentLayerStream.clear();
                }

                if (!currentFrame.empty()) {
                    LayeredFrame lf;
                    lf.layers = currentFrame;
                    resultFrames.push_back(lf);
                    currentFrame.clear();
                }
            }
            else if (line == "---LAYER---")
            {
                if (!currentLayerStream.str().empty())
                {
                    currentFrame.push_back(currentLayerStream.str());
                    currentLayerStream.str("");
                    currentLayerStream.clear();
                }
            }
            else
            {
                currentLayerStream << line << '\n';
            }
        }

        if (!currentLayerStream.str().empty())
        {
            currentFrame.push_back(currentLayerStream.str());
        }

        if (!currentFrame.empty())
        {
            LayeredFrame lf;
            lf.layers = currentFrame;
            resultFrames.push_back(lf);
        }


        return resultFrames;
    }


    TextureID FileLoader::LoadTileset(const std::string& filename, int tileSize) {
        int width, height, channels;
        std::string resolvedPath = filename;

        if (!fs::exists(filename)) {
            resolvedPath = FindFileInRes(filename);
            if (resolvedPath.empty()) {
                FX_CORE_WARN("Tileset file not found: {}", filename);
                return 0;
            }
        }

        unsigned char* data = stbi_load(resolvedPath.c_str(), &width, &height, &channels, 4);
        if (!data) {
            FX_CORE_WARN("Failed to load tileset image: {}", resolvedPath);
            return 0;
        }

        /*this->tileSize = tileSize;
        tilesetWidth = width;
        tilesetHeight = height;
        tilesPerRow = tilesetWidth / tileSize;*/ //Was in TileRenderer in Prototyp (was used in RenderToTexture Function in Prototype)

        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        stbi_image_free(data);

        return textureID;
    }

}

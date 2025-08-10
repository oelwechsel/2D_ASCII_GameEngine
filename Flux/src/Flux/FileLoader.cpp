#include "fxpch.h"
#include "Flux/FileLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "GL/glew.h"


namespace Flux
{
    namespace fs = std::filesystem;

    static std::string FindFileInRes(const std::string& _filename)
    {
        const fs::path baseDir = fs::current_path() / "src" / "res";

        for (auto& p : fs::recursive_directory_iterator(baseDir))
        {
            if (!p.is_regular_file()) continue;

            if (p.path().filename() == _filename)
                return p.path().string();
        }

        return ""; 
    }

    static std::string FindFileInHome(const std::string& _filename)
    {
        const fs::path baseDir = fs::current_path() / "home";

        for (auto& p : fs::recursive_directory_iterator(baseDir))
        {
            if (!p.is_regular_file()) continue;

            if (p.path().filename() == _filename)
                return p.path().string();
        }

        return "";
    }

    std::vector<std::string> FileLoader::LoadTextFile(const std::string& _filename, bool* _success)
    {
        std::string resolvedPath = _filename;

        if (!fs::exists(_filename))
        {
            resolvedPath = FindFileInRes(_filename);

            if (resolvedPath.empty())
            {
                FX_CORE_WARN("Text file not found: {}", _filename);
                if (_success) *_success = false;
                return {};
            }
        }

        std::ifstream file(resolvedPath);
        if (!file.is_open())
        {
            FX_CORE_WARN("Failed to open text file: {}", resolvedPath);
            if (_success) *_success = false;
            return {};
        }

        FX_CORE_INFO("Loading Text File: {}", resolvedPath);

        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line))
        {
            lines.push_back(line);
        }

        if (_success) *_success = true;
        return lines;
    }

    unsigned int FileLoader::LoadTextureFromFile(const std::string& _filename, int _desiredChannels)
    {
        std::string resolvedPath = _filename;

        if (!fs::exists(_filename))
        {
            resolvedPath = FindFileInRes(_filename);
            if (resolvedPath.empty())
            {
                FX_CORE_WARN("File not found: {}", _filename);
                return 0;
            }
        }

        int width, height, channels;
        unsigned char* data = stbi_load(resolvedPath.c_str(), &width, &height, &channels, _desiredChannels);
        if (!data)
        {
            std::cerr << "Failed to load image: " << resolvedPath << std::endl;
            return 0;
        }

        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
            _desiredChannels == 4 ? GL_RGBA : GL_RGB,
            GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);

        return textureID;
    }

    std::vector<LayeredFrame> FileLoader::LoadAsciiFrames(const std::string& _filename)
    {
        std::string resolvedPath = _filename;

        if (!std::filesystem::exists(_filename))
        {
            resolvedPath = FindFileInRes(_filename);
            if (resolvedPath.empty())
            {
                FX_CORE_WARN("AsciiFrame file not found: {}", _filename);
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


    TextureID FileLoader::LoadTileset(const std::string& _filename, int _tileSize) 
    {
        int width, height, channels;
        std::string resolvedPath = _filename;

        if (!fs::exists(_filename)) 
        {
            resolvedPath = FindFileInRes(_filename);
            if (resolvedPath.empty()) 
            {
                FX_CORE_WARN("Tileset file not found: {}", _filename);
                return 0;
            }
        }

        unsigned char* data = stbi_load(resolvedPath.c_str(), &width, &height, &channels, 4);
        if (!data) 
        {
            FX_CORE_WARN("Failed to load tileset image: {}", resolvedPath);
            return 0;
        }

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

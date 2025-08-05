#pragma once

#include "Flux/Core.h"
#include "../vendor/imgui/imgui.h"

namespace Flux
{

    class FLUX_API Entity 
    {
    public:
        int x, y;
        char ascii;
        std::string name;
        bool isPlayer;
        std::vector<std::string> dialogueLines;
        std::vector<std::string> animationFrames;
        //std::vector<LayeredFrame> layeredFrames;
        float frameDuration;
        ImVec2 windowSize;

        Entity(int x_, int y_, char ascii_, std::string name_, float frameDuration_, ImVec2 windowSize_ = ImVec2(230, 350), bool isPlayer_ = false);
    };
}

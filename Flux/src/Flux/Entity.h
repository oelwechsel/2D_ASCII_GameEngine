#pragma once

#include "Flux/Core.h"
#include "../vendor/imgui/imgui.h"

namespace Flux
{
    struct FLUX_API LayeredFrame {
        std::vector<std::string> layers;
    };

    class FLUX_API Entity 
    {
    public:
        int x, y;
        char ascii;
        std::string name;
        bool isPlayer;
        std::vector<std::string> dialogueLines;
        std::vector<std::string> animationFrames;
        std::vector<LayeredFrame> layeredFrames;
        float frameDuration;
        ImVec2 windowSize;
        std::function<void()> afterInteraction;
        std::function<void()> onInteract;
        bool hasInteractedWith = false;
        std::string dirName;
        ImVec4 logColor;

        Entity(int x_, int y_, char ascii_, std::string name_, std::string dirName_, ImVec4 logColor_, float frameDuration_, ImVec2 windowSize_ = ImVec2(230, 350), bool isPlayer_ = false);
    };
}

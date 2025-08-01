#pragma once
#include <Flux.h>
#include "internal/BlockData.h"


// Change Later!
struct LayeredFrame {
    std::vector<std::string> layers;
};


struct Entity {
    int x, y;
    char ascii;
    std::string name;
    bool isPlayer = false;
    std::vector<std::string> dialogueLines;
    std::vector<std::string> animationFrames;
    std::vector<LayeredFrame> layeredFrames;
    float frameDuration = 0.2f;
    ImVec2 windowSize;

    Entity(int x_, int y_, char ascii_, std::string name_, float frameDuration_, ImVec2 windowSize_ = ImVec2(230, 350), bool isPlayer_ = false)
        : x(x_), y(y_), ascii(ascii_), name(name_), frameDuration(frameDuration_), windowSize(windowSize_), isPlayer(isPlayer_) {
    }
};



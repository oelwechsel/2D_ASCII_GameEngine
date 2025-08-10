#include "fxpch.h"
#include "Entity.h"


namespace Flux
{
    Entity::Entity(int x_, int y_, char ascii_, std::string name_, std::string dirName_, float frameDuration_, ImVec2 windowSize_, bool isPlayer_)
        : x(x_), y(y_), ascii(ascii_), name(std::move(name_)), dirName(dirName_), frameDuration(frameDuration_), windowSize(windowSize_), isPlayer(isPlayer_) {
    }
}

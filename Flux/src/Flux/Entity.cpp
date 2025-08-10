#include "fxpch.h"
#include "Entity.h"


namespace Flux
{
    Entity::Entity(int x_, int y_, char ascii_, std::string name_, std::string dirName_, ImVec4 logColor_, float frameDuration_, ImVec2 windowSize_, bool isPlayer_)
        : x(x_)
        , y(y_)
        , ascii(ascii_)
        , name(std::move(name_))
        , dirName(std::move(dirName_))
        , logColor(logColor_)        
        , frameDuration(frameDuration_)
        , windowSize(windowSize_)
        , isPlayer(isPlayer_)
    {
    }

}

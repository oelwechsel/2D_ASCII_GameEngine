#include "fxpch.h"
#include "Entity.h"


namespace Flux
{
    Entity::Entity(int _xPos, int _yPos, char ascii_, std::string _AsciiName, std::string _dirName, ImVec4 _logColor, float _frameDuration, ImVec2 _windowSize, bool _isPlayer)
        : m_xPos(_xPos)
        , m_yPos(_yPos)
        , m_AsciiName(ascii_)
        , m_Name(std::move(_AsciiName))
        , m_DirName(std::move(_dirName))
        , m_LogColor(_logColor)        
        , m_FrameDuration(_frameDuration)
        , m_WindowSize(_windowSize)
        , m_IsPlayer(_isPlayer)
    {
    }
}

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
        int m_xPos, m_yPos;
        char m_AsciiName;
        std::string m_Name;
        bool m_IsPlayer;
        std::vector<std::string> m_DialogueLines;
        std::vector<std::string> m_AnimationFrames;
        std::vector<LayeredFrame> m_LayeredFrames;
        float m_FrameDuration;
        ImVec2 m_WindowSize;
        std::function<void()> afterInteraction;
        std::function<void()> onInteract;
        bool m_HasInteractedWith = false;
        std::string m_DirName;
        ImVec4 m_LogColor;

        Entity(int _xPos, int _yPos, char _AsciiName, std::string _Name, std::string _dirName, ImVec4 _logColor, float _frameDuration, ImVec2 _windowSize = ImVec2(230, 350), bool _isPlayer = false);
    };
}

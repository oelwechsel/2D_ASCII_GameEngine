#pragma once
#include "Flux/Core.h"
#include "../vendor/imgui/imgui.h"

namespace Flux::ImGuiWrapper 
{
    FLUX_API void Begin(const char* _name,
        const ImVec2& _size = ImVec2(0, 0),
        const ImVec2& _pos = ImVec2(0, 0),
        ImGuiWindowFlags _flags = 0,
        ImGuiCond _posCond = ImGuiCond_Once,
        ImGuiCond _sizeCond = ImGuiCond_Once);

	FLUX_API void Text(const char* fmt, ...);
    FLUX_API void Text(const std::string& _str);
    FLUX_API void Text(const std::vector<std::string>& _lines);
	FLUX_API void End();
	FLUX_API bool Button(const char* _buttonName);
    FLUX_API void Image(unsigned int _textureID, ImVec2 _size);

    FLUX_API float GetFontSize();

    FLUX_API ImVec2 GetCursorScreenPos();

    FLUX_API ImDrawList* GetWindowDrawList();

    FLUX_API void AddText(ImDrawList* _drawList, const ImVec2& _pos, ImU32 _color, const char* _text);

    FLUX_API void AnimateLayers(ImDrawList* _drawList,
                  ImVec2 _startPos,
                  const std::vector<std::string>& _layers,
                  const std::vector<ImU32>& _layerColors); 

    FLUX_API ImVec2 GetScale();

    FLUX_API void LoadCustomFont(const std::string& _fontPath, float _fontSize, const std::string& _extraChars);

    FLUX_API ImVec2 GetContentRegionAvail();
}



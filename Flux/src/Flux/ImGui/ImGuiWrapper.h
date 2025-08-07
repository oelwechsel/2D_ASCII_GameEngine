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
    FLUX_API void Text(const std::string& str);
    FLUX_API void Text(const std::vector<std::string>& lines);
	FLUX_API void End();
	FLUX_API bool Button(const char* _buttonName);
    FLUX_API void Image(unsigned int textureID, ImVec2 size);

    FLUX_API float GetFontSize();

    FLUX_API ImVec2 GetCursorScreenPos();

    FLUX_API ImDrawList* GetWindowDrawList();

    FLUX_API void AddText(ImDrawList* drawList, const ImVec2& pos, ImU32 color, const char* text);

    FLUX_API void AnimateLayers(ImDrawList* drawList,
                  ImVec2 startPos,
                  const std::vector<std::string>& layers,
                  const std::vector<ImU32>& layerColors); 

    FLUX_API ImVec2 GetScale();
}



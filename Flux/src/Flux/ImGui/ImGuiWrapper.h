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
	FLUX_API void Button(const char* _buttonName);
}



#include "fxpch.h"
#include "ImGuiWrapper.h"
#include "imgui.h"

namespace Flux::ImGuiWrapper 
{

	void Begin(
		const char* _name,
		ImGuiWindowFlags _flags = 0,
		ImVec2* _size = nullptr,
		ImVec2* _pos = nullptr,
		ImGuiCond _posCond = ImGuiCond_Once,
		ImGuiCond _sizeCond = ImGuiCond_Once)
	{
		if (_pos)
			ImGui::SetNextWindowPos(*_pos, _posCond);
		if (_size)
			ImGui::SetNextWindowSize(*_size, _sizeCond);

		ImGui::Begin(_name, nullptr, _flags);
	}

	void Text(const char* fmt, ...) 
	{
		va_list args;
		va_start(args, fmt);
		ImGui::TextV(fmt, args);
		va_end(args);
	}

	void End() 
	{ 
		ImGui::End(); 
	}

	void Button(const char* _buttonName)
	{
		ImGui::Button(_buttonName);
	}
}
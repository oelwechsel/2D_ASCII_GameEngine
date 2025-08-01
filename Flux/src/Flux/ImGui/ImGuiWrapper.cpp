#include "fxpch.h"
#include "ImGuiWrapper.h"


namespace Flux::ImGuiWrapper 
{
	void Begin(const char* _name,
		const ImVec2& _size,
		const ImVec2& _pos,
		ImGuiWindowFlags _flags,
		ImGuiCond _posCond,
		ImGuiCond _sizeCond)
	{
		if (_pos.x != 0 || _pos.y != 0)
			ImGui::SetNextWindowPos(_pos, _posCond);
		if (_size.x != 0 || _size.y != 0)
			ImGui::SetNextWindowSize(_size, _sizeCond);

		ImGui::Begin(_name, nullptr, _flags);
	}

	void Text(const char* fmt, ...) 
	{
		va_list args;
		va_start(args, fmt);
		ImGui::TextV(fmt, args);
		va_end(args);
	}
	
	// String
	void Text(const std::string& str)
	{
		ImGui::TextUnformatted(str.c_str());
	}

	// vector<string>
	void Text(const std::vector<std::string>& lines)
	{
		for (const auto& line : lines)
		{
			ImGui::TextUnformatted(line.c_str());
		}
	}

	void End() 
	{ 
		ImGui::End(); 
	}

	void Button(const char* _buttonName)
	{
		ImGui::Button(_buttonName);
	}

	void Image(unsigned int textureID, ImVec2 size)
	{
		ImGui::Image((ImTextureID)textureID, size);
	}

}
#include "fxpch.h"
#include "ImGuiWrapper.h"
#include "imgui_internal.h"
#include "../Log.h"

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

	bool Button(const char* _buttonName)
	{
		return ImGui::Button(_buttonName);
	}

	void Image(unsigned int textureID, ImVec2 size)
	{
		ImGui::Image((ImTextureID)textureID, size);
	}

	float ImGuiWrapper::GetFontSize()
	{
		return ImGui::GetFontSize();
	}

	ImVec2 ImGuiWrapper::GetCursorScreenPos()
	{
		return ImGui::GetCursorScreenPos();
	}

	ImDrawList* ImGuiWrapper::GetWindowDrawList()
	{
		return ImGui::GetWindowDrawList();
	}

	void ImGuiWrapper::AddText(ImDrawList* drawList, const ImVec2& pos, ImU32 color, const char* text)
	{
		drawList->AddText(pos, color, text);
	}

	inline void ImGuiWrapper::AnimateLayers(ImDrawList* drawList, ImVec2 startPos,
		const std::vector<std::string>& layers,
		const std::vector<ImU32>& layerColors)
	{
		float lineHeight = GetFontSize() + 1.0f;

		for (size_t layerIndex = 0; layerIndex < layers.size(); ++layerIndex)
		{
			const std::string& layerText = layers[layerIndex];
			float y = startPos.y;

			std::istringstream iss(layerText);
			std::string line;
			while (std::getline(iss, line))
			{
				ImU32 color = layerColors.size() > layerIndex ? layerColors[layerIndex] : layerColors.back();

				AddText(drawList, ImVec2(startPos.x, y), color, line.c_str());

				y += lineHeight;
			}
		}
	}

	ImVec2 ImGuiWrapper::GetScale() {
		return ImGui::GetIO().DisplayFramebufferScale;
	}

	void ImGuiWrapper::LoadCustomFont(const std::string& fontPath, float fontSize, const std::string& extraChars)
	{
		ImGuiIO& io = ImGui::GetIO();

		ImFontGlyphRangesBuilder builder;
		builder.AddRanges(io.Fonts->GetGlyphRangesDefault());
		builder.AddText(extraChars.c_str());  // ⬅️ Hier fügst du deine eigenen Zeichen hinzu

		ImVector<ImWchar> ranges;
		builder.BuildRanges(&ranges);

		ImFont* font = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), fontSize, nullptr, ranges.Data);
		if (!font)
		{
			FX_WARN("[ImGuiWrapper] Failed to load font from path: {}", fontPath);
		}
		else
		{
			FX_INFO("[ImGuiWrapper] Successfully loaded font: {}", fontPath);
		}
	}

}
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

	void Text(const std::string& str)
	{
		ImGui::TextUnformatted(str.c_str());
	}

	void Text(const std::vector<std::string>& _lines)
	{
		for (const auto& line : _lines)
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

	void Image(unsigned int _textureID, ImVec2 _size)
	{
		ImGui::Image((ImTextureID)_textureID, _size);
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

	void ImGuiWrapper::AddText(ImDrawList* _drawList, const ImVec2& _pos, ImU32 _color, const char* _text)
	{
		_drawList->AddText(_pos, _color, _text);
	}

	inline void ImGuiWrapper::AnimateLayers(ImDrawList* _drawList, ImVec2 _startPos,
		const std::vector<std::string>& layers,
		const std::vector<ImU32>& layerColors)
	{
		float lineHeight = GetFontSize() + 1.0f;

		for (size_t layerIndex = 0; layerIndex < layers.size(); ++layerIndex)
		{
			const std::string& layerText = layers[layerIndex];
			float y = _startPos.y;

			std::istringstream iss(layerText);
			std::string line;
			while (std::getline(iss, line))
			{
				ImU32 color = layerColors.size() > layerIndex ? layerColors[layerIndex] : layerColors.back();

				AddText(_drawList, ImVec2(_startPos.x, y), color, line.c_str());

				y += lineHeight;
			}
		}
	}

	ImVec2 ImGuiWrapper::GetScale() {
		return ImGui::GetIO().DisplayFramebufferScale;
	}

	void ImGuiWrapper::LoadCustomFont(const std::string& _fontPath, float _fontSize, const std::string& _extraChars)
	{
		ImGuiIO& io = ImGui::GetIO();

		ImFontGlyphRangesBuilder builder;
		builder.AddRanges(io.Fonts->GetGlyphRangesDefault());
		builder.AddText(_extraChars.c_str()); 

		ImVector<ImWchar> ranges;
		builder.BuildRanges(&ranges);

		ImFont* font = io.Fonts->AddFontFromFileTTF(_fontPath.c_str(), _fontSize, nullptr, ranges.Data);
		if (!font)
		{
			FX_WARN("[ImGuiWrapper] Failed to load font from path: {}", _fontPath);
		}
		else
		{
			FX_INFO("[ImGuiWrapper] Successfully loaded font: {}", _fontPath);
		}
	}

	ImVec2 ImGuiWrapper::GetContentRegionAvail() {
		return ImGui::GetContentRegionAvail();
	}
}
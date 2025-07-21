#include "fxpch.h"
#include "ImGuiWrapper.h"
#include "imgui.h"

namespace Flux::ImGuiWrapper 
{

	void Begin(const char* name) 
	{ 
		ImGui::Begin(name); 
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
}
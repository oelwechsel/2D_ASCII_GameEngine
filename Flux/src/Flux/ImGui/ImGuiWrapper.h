#pragma once

#include "Flux/Core.h"

namespace Flux::ImGuiWrapper 
{
	FLUX_API void Begin(const char* name);
	FLUX_API void Text(const char* fmt, ...);
	FLUX_API void End();
	FLUX_API void Button(const char* _buttonName);
}



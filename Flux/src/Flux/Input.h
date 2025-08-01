#pragma once

#include "Flux/Core.h"

namespace Flux
{
	class FLUX_API Input
	{
	public:
		inline static bool IsKeyPressed(int _keycode);

		inline static bool IsMouseButtonPressed(int _button);
		inline static std::pair<float, float> GetMousePosition();
		inline static float GetMouseX();
		inline static float GetMouseY();

	private:
		static Input* s_Instance;
	};
}

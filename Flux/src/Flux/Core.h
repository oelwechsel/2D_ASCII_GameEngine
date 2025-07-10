#pragma once

#ifdef FX_PLATFORM_WINDOWS
	#ifdef FX_BUILD_DLL
		#define FLUX_API __declspec(dllexport)
	#else
		#define FLUX_API __declspec(dllimport)
	#endif 
#else
	#error Flux Engine is only Supporting Windows!
#endif

#define BIT(x) (1 << x) //Bitfield -> Event can have multiple Categories (e.g. MouseButtonEvent is also a MouseEvent)

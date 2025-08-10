#pragma once

#ifdef FX_PLATFORM_WINDOWS
	#ifdef FX_BUILD_DLL
		#define FLUX_API __declspec(dllexport)
	#else
		#define FLUX_API __declspec(dllimport)
	#endif 
#else
	#error Flux Engine is Windows only for now
#endif

#ifdef FX_ENABLE_ASSERTS
    #define FX_ASSERT(x, ...) {if(!(x)) {FX_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();} }
    #define FX_CORE_ASSERT(x, ...) {if(!(x)) {FX_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();} }
#else
    #define FX_ASSERT(x, ...)
    #define FX_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x) //Bitfield -> Event can have multiple Categories (e.g. MouseButtonEvent is also a MouseEvent)

#define FX_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

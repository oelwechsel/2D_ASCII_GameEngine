#pragma once

#include <memory>
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Flux 
{

	class FLUX_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger;}
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger;}

	private:
		 static std::shared_ptr<spdlog::logger> s_CoreLogger;
		 static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core log macros
#define FX_CORE_TRACE(...)			::Flux::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define FX_CORE_INFO(...)			::Flux::Log::GetCoreLogger()->info(__VA_ARGS__)
#define FX_CORE_WARN(...)			::Flux::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define FX_CORE_ERROR(...)			::Flux::Log::GetCoreLogger()->error(__VA_ARGS__)
#define FX_CORE_CRITICAL(...)		::Flux::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define FX_TRACE(...)					::Flux::Log::GetClientLogger()->trace(__VA_ARGS__)
#define FX_INFO(...)					::Flux::Log::GetClientLogger()->info(__VA_ARGS__)
#define FX_WARN(...)					::Flux::Log::GetClientLogger()->warn(__VA_ARGS__)
#define FX_ERROR(...)					::Flux::Log::GetClientLogger()->error(__VA_ARGS__)
#define FX_CRITICAL(...)				::Flux::Log::GetClientLogger()->critical(__VA_ARGS__)
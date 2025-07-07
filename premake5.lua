workspace "Flux"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Flux"
	location "Flux"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "10.0.22621.0"
		buildoptions { "/utf-8" }

		defines
		{
			"FX_PLATFORM_WINDOWS",
			"FX_BUILD_DLL",
		}

		postbuildcommands
		{
			'{COPYFILE} "%{cfg.buildtarget.relpath}" "../bin/' .. outputdir .. '/@venture/"'
		}


	filter "configurations:Debug"
		defines "FX_DEBUG"
		symbols "On"
		
	filter "configurations:Release"
		defines "FX_RELEASE"
		optimize "On"
		
	filter "configurations:Dist"
		defines "FX_DIST"
		optimize "On"



project "@venture"
	location "@venture"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Flux/vendor/spdlog/include",
		"Flux/src"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "10.0.22621.0"
		buildoptions { "/utf-8" }

		defines
		{
			"FX_PLATFORM_WINDOWS"
		}

		links
		{
			"Flux"
		}

	filter "configurations:Debug"
		defines "FX_DEBUG"
		symbols "On"
		
	filter "configurations:Release"
		defines "FX_RELEASE"
		optimize "On"
		
	filter "configurations:Dist"
		defines "FX_DIST"
		optimize "On"
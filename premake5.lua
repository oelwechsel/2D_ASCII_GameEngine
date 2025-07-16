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

	pchheader "fxpch.h"
	pchsource "Flux/src/fxpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/GLFW/include",
		"%{prj.name}/vendor/GLEW/include"
	}

	libdirs 
	{
        "%{prj.name}/vendor/GLFW/lib-vc2022",
		"%{prj.name}/vendor/GLEW/lib"
    }

	links 
	{
        "glfw3_mt",
		"glew32s.lib",
        "opengl32.lib"
    }

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "Off" -- On? 
		systemversion "latest"
		buildoptions { "/utf-8" }

		defines
		{
			"FX_PLATFORM_WINDOWS",
			"FX_BUILD_DLL",
			"GLFW_STATIC",
			"GLEW_STATIC",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			'{COPYFILE} "%{cfg.buildtarget.relpath}" "../bin/' .. outputdir .. '/@venture/"'
		}


	filter "configurations:Debug"
		defines "FX_DEBUG"
		-- buildoptions "/MDd"
		symbols "On"
		
	filter "configurations:Release"
		defines "FX_RELEASE"
		-- buildoptions "/MD"
		optimize "On"
		
	filter "configurations:Dist"
		defines "FX_DIST"
		-- buildoptions "/MD"
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
		"Flux/vendor/GLFW/include",
		"Flux/vendor/GLEW/include",
		"Flux/src"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "Off" -- On? 
		systemversion "latest"
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
		-- buildoptions "/MDd"
		symbols "On"
		
	filter "configurations:Release"
		defines "FX_RELEASE"
		-- buildoptions "/MD"
		optimize "On"
		
	filter "configurations:Dist"
		defines "FX_DIST"
		-- buildoptions "/MD"
		optimize "On"
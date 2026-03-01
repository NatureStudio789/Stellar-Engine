workspace "Stellar-Engine"
	architecture "x64"
	startproject "Stellar Engine"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

	flags
	{
		"MultiProcessorCompile"
	}

OutputDirectory = "%{cfg.buildcfg} - %{cfg.system}/%{cfg.architecture}"

IncludeDependenciesDir = "Dependencies/Includes/"
LibraryDependenciesDir = "Dependencies/Libraries/"
BinaryDependenciesDir = "Dependencies/Binaries/"

include "Third Party/GLFW/GLFW.lua"
include "Third Party/imgui/imgui.lua"

project "Stellar Engine"
	location "./"
	kind "ConsoleApp"
	language "C++"

	targetdir("Build/" .. OutputDirectory)
	objdir("%{prj.name}/Intermediate/" .. OutputDirectory)

	pchheader "Core.h"
	pchsource "Engine/Sources/Runtime/Core/Core.cpp"

	files
	{
		"Engine/Sources/Runtime/**.h",
		"Engine/Sources/Runtime/**.hpp",
		"Engine/Sources/Runtime/**.cpp",

		"Engine/Sources/Editor/**.h",
		"Engine/Sources/Editor/**.hpp",
		"Engine/Sources/Editor/**.cpp",
	}

	includedirs
	{
		"Engine/Sources/Runtime/Core/",

		IncludeDependenciesDir,
		"Third Party/GLFW/include/",
		"Third Party/imgui/"
	}

	libdirs
	{
		LibraryDependenciesDir .. "DirectXTex/%{cfg.buildcfg}/",
	}

	links
	{
		"GLFW",
		"imgui",

		"dxgi",
		"d3d12",
		"d3dcompiler",
		"dxguid",

		"rpcrt4",

		"DirectXTex.lib",
	}

	defines
	{
		"SE_BUILD_ENGINE",
	}

	filter "system:Windows"
		cppdialect "C++20"
		staticruntime "Off"
		systemversion "latest"

		defines
		{
			"SE_PLATFORM_WINDOWS",
		}

	filter "system:Linux"
		cppdialect "C++20"
		staticruntime "Off"
		systemversion "latest"

		defines
		{
			"SE_PLATFORM_LINUX",
		}

	filter "system:macosx"
		cppdialect "C++20"
		staticruntime "Off"
		systemversion "latest"

		defines
		{
			"SE_PLATFORM_MACOS",
		}

	filter "configurations:Debug"
		runtime "Debug"
		defines
		{
			"SE_DEBUG"
		}
		symbols "On"

        buildoptions {"/wd4251", "/MP"}
        buildoptions {"/wd4275", "/MP"}
        buildoptions {"/wd4819", "/MP"}
        buildoptions {"/wd4099", "/MP"}
        buildoptions {"/wd4996", "/MP"}
        buildoptions { "/permissive" }

	filter "configurations:Release"
		runtime "Release"
		defines
		{
			"SE_RELEASE"
		}
		optimize "On"

	filter "configurations:Distribution"
		defines
		{
			"SE_DISTRIBUTION"
		}
		optimize "On"

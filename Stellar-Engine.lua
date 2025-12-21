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

project "Stellar Engine"
	location "./"
	kind "ConsoleApp"
	language "C++"

	targetdir("Build/" .. OutputDirectory)
	objdir("%{prj.name}/Intermediate/" .. OutputDirectory)

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
		IncludeDependenciesDir,
	}

	libdirs
	{
		LibrariesDependenciesDir .. "GLFW/",
	}

	links
	{
		"glfw3.lib",
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

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
	location "Stellar Engine"
	kind "ConsoleApp"
	language "C++"

	targetdir("Build/" .. OutputDirectory)
	objdir("%{prj.name}/Intermediate/" .. OutputDirectory)

	files
	{
		"%{prj.name}/Sources/**.h",
		"%{prj.name}/Sources/**.hpp",
		"%{prj.name}/Sources/**.cpp",
	}

	includedirs
	{

	}

	libdirs
	{

	}

	links
	{

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

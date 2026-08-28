project "yaml-cpp"
	kind "StaticLib"
	language "C++"
    staticruntime "off"

	targetdir ("../../Build/" .. OutputDirectory)
    objdir ("Intermediate/" .. OutputDirectory)

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"include/"
	}

    defines
    {
        "YAML_CPP_STATIC_DEFINE",
    }

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++20"
		staticruntime "Off"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Distribution"
		runtime "Release"
		optimize "on"
        symbols "off"
#ifndef _SE_CORE_H_
#define _SE_CORE_H_
#include <string>
#include <cmath>
#include <iostream>
#include <assert.h>
#include <memory>
#include <map>

#include <GLFW/glfw3.h>
#ifdef SE_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <Windows.h>
#else
#error Stellar Engine only supports Windows!!!!
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectX 12/d3dx12.h>

#include <backends/imgui_impl_dx12.h>
#include <imgui.h>

#include <wrl/client.h>
using namespace Microsoft;

// Define nullptr to null for convenience.
#define null nullptr

// Struct memory clearing function.
#define STELLAR_CLEAR_MEMORY(Variable) ZeroMemory(&Variable, sizeof(Variable))

// Calling for singleton.
#define STELLAR_MAKE_SINGLETON(Class, Name) static std::unique_ptr<Class> ##Name;
#define STELLAR_FINE_SINGLETON(Class, Name) std::unique_ptr<Class> Class::##Name = std::make_unique<Class>();

namespace SE
{

}

/*

- Class Naming Rules:
	Meanings Of Prefix Of Classes:
		"S" - Core module's classes, which is low-level.
		"F" - Function classes.
		"G" - Graphics module's classes.

- Platform Defines:
	Windows: SE_PLATFORM_WINDOWS
	Linux: SE_PLATFORM_LINUX
	MacOSX: SE_PLATFORM_MACOS

- Configuration Defines:
	Debug: SE_DEBUG
	Release: SE_RELEASE
	Distribution: SE_DISTRIBUTION

*/

#endif
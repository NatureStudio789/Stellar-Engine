#ifndef _SE_CORE_H_
#define _SE_CORE_H_
#include <string>
#include <cmath>
#include <iostream>
#include <assert.h>
#include <memory>

#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
#include <xmmintrin.h>
#elif defined(__aarch64__) || defined(_M_ARM64) || defined(__arm__) || defined(_M_ARM)
#include <arm_neon.h>
#endif

#include <GLFW/glfw3.h>
#ifdef SE_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <Windows.h>
#endif

// Define nullptr to null for convenience.
#define null nullptr

// Calling for singleton.
#define STELLAR_MAKE_SINGLETON(Class, Name) static std::unique_ptr<Class> ##Name;
#define STELLAR_FINE_SINGLETON(Class, Name) std::unique_ptr<Class> Class::##Name = std::make_unique<Class>();

namespace SE
{
	// Here are some basic stellar types.
	using SInt = int;
	using SInt32 = int;
	using SUInt = unsigned int;
	using SUInt32 = unsigned int;

	using SLong = long;
	using SULong = unsigned long;

	using SLongLong64 = long long;
	using SULongLong64 = unsigned long long;

	using SFloat = float;
	using SDouble = double;

	using SChar = char;
	using SUChar = unsigned char;
	using SByte = SUChar;

	using SBool = bool;

	using SString = std::string;
	using SWString = std::wstring;

	class SUtil
	{
	public:
		static void AddSIMD(const SFloat a[4], const SFloat b[4], SFloat* out);
		static void MultiplySIMD(const SFloat a[4], const SFloat b[4], SFloat* out);
		static void DivideSIMD(const SFloat a[4], const SFloat b[4], SFloat* out);
	};
}

/*

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
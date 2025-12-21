#ifndef _SE_CORE_H_
#define _SE_CORE_H_
#include <string>
#include <cmath>
#include <iostream>

#include <assert.h>

#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
#include <xmmintrin.h>
#elif defined(__aarch64__) || defined(_M_ARM64) || defined(__arm__) || defined(_M_ARM)
#include <arm_neon.h>
#endif

// Define nullptr to null for convenience.
#define null nullptr

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

#endif
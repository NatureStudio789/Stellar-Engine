#include "Core.h"

namespace SE
{
	void SUtil::AddSIMD(const SFloat a[4], const SFloat b[4], SFloat* out)
	{
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)

		// SSE arch SIMD calculation.
		__m128 A = _mm_loadu_ps(a);
		__m128 B = _mm_loadu_ps(b);
		__m128 RESULT = _mm_add_ps(A, B);

		_mm_storeu_ps(out, RESULT);

#elif defined(__aarch64__) || defined(_M_ARM64) || defined(__arm__) || defined(_M_ARM)

		// TO DO: Finish the SIMD for ARM architecture.
		
#endif
	}

	void SUtil::MultiplySIMD(const SFloat a[4], const SFloat b[4], SFloat* out)
	{
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)

		__m128 A = _mm_loadu_ps(a);
		__m128 B = _mm_loadu_ps(b);
		__m128 RESULT = _mm_mul_ps(A, B);

		_mm_storeu_ps(out, RESULT);

#elif defined(__aarch64__) || defined(_M_ARM64) || defined(__arm__) || defined(_M_ARM)



#endif
	}

	void SUtil::DivideSIMD(const SFloat a[4], const SFloat b[4], SFloat* out)
	{
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)

		__m128 A = _mm_loadu_ps(a);
		__m128 B = _mm_loadu_ps(b);
		__m128 RESULT = _mm_div_ps(A, B);

		_mm_storeu_ps(out, RESULT);

#elif defined(__aarch64__) || defined(_M_ARM64) || defined(__arm__) || defined(_M_ARM)



#endif
	}
}

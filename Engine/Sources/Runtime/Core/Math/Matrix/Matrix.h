#ifndef _SE_MATRIX_H_
#define _SE_MATRIX_H_
#include "../../Core.h"

namespace SE
{
	class FMatrix4x4
	{
	public:
		FMatrix4x4();
		FMatrix4x4(const FMatrix4x4& other);
		FMatrix4x4(SFloat m00, SFloat m01, SFloat m02, SFloat m03,
				   SFloat m10, SFloat m11, SFloat m12, SFloat m13, 
				   SFloat m20, SFloat m21, SFloat m22, SFloat m23, 
				   SFloat m30, SFloat m31, SFloat m32, SFloat m33);
		FMatrix4x4(const SFloat mat[4][4]);
		FMatrix4x4(const SFloat array[16]);
		~FMatrix4x4();

		void Initialize(const FMatrix4x4& other);
		void Initialize(SFloat m00, SFloat m01, SFloat m02, SFloat m03,
						SFloat m10, SFloat m11, SFloat m12, SFloat m13,
						SFloat m20, SFloat m21, SFloat m22, SFloat m23,
						SFloat m30, SFloat m31, SFloat m32, SFloat m33);
		void Initialize(const SFloat mat[4][4]);
		void Initialize(const SFloat array[16]);

		FMatrix4x4& Multiply(const FMatrix4x4& other);

		FMatrix4x4& Identity();
		FMatrix4x4& Transpose();

		FMatrix4x4 operator*(const FMatrix4x4& other) const noexcept;
		FMatrix4x4& operator*=(const FMatrix4x4& other);

		const float* operator[](SUInt index) const noexcept;
		float* operator[](SUInt index);

	public:
		SFloat Mat[4][4];
	};

	using FMatrix = FMatrix4x4;
}

#endif
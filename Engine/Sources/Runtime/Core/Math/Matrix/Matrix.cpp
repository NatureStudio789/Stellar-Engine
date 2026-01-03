#include "Matrix.h"

namespace SE
{
	FMatrix4x4::FMatrix4x4()
	{

	}

	FMatrix4x4::FMatrix4x4(const FMatrix4x4& other)
	{
		this->Initialize(other);
	}

	FMatrix4x4::FMatrix4x4(SFloat m00, SFloat m01, SFloat m02, SFloat m03, 
						   SFloat m10, SFloat m11, SFloat m12, SFloat m13, 
						   SFloat m20, SFloat m21, SFloat m22, SFloat m23, 
						   SFloat m30, SFloat m31, SFloat m32, SFloat m33)
	{
		this->Initialize(m00, m01, m02, m03,
						 m10, m11, m12, m13,
						 m20, m21, m22, m23,
						 m30, m31, m32, m33);
	}

	FMatrix4x4::FMatrix4x4(const SFloat mat[4][4])
	{
		this->Initialize(mat);
	}

	FMatrix4x4::FMatrix4x4(const SFloat array[16])
	{
		this->Initialize(array);
	}

	FMatrix4x4::~FMatrix4x4()
	{

	}

	void FMatrix4x4::Initialize(const FMatrix4x4& other)
	{
		for (SUInt i = 0; i < 4; i++)
		{
			for (SUInt j = 0; j < 4; j++)
			{
				this->Mat[i][j] = other.Mat[i][j];
			}
		}
	}

	void FMatrix4x4::Initialize(SFloat m00, SFloat m01, SFloat m02, SFloat m03, 
							    SFloat m10, SFloat m11, SFloat m12, SFloat m13, 
							    SFloat m20, SFloat m21, SFloat m22, SFloat m23, 
							    SFloat m30, SFloat m31, SFloat m32, SFloat m33)
	{
		this->Mat[0][0] = m00;
		this->Mat[0][1] = m01;
		this->Mat[0][2] = m02;
		this->Mat[0][3] = m03;
		this->Mat[1][0] = m10;
		this->Mat[1][1] = m11;
		this->Mat[1][2] = m12;
		this->Mat[1][3] = m13;
		this->Mat[2][0] = m20;
		this->Mat[2][1] = m21;
		this->Mat[2][2] = m22;
		this->Mat[2][3] = m23;
		this->Mat[3][0] = m30;
		this->Mat[3][1] = m31;
		this->Mat[3][2] = m32;
		this->Mat[3][3] = m33;
	}

	void FMatrix4x4::Initialize(const SFloat mat[4][4])
	{
		for (SUInt i = 0; i < 4; i++)
		{
			for (SUInt j = 0; j < 4; j++)
			{
				this->Mat[i][j] = mat[i][j];
			}
		}
	}

	void FMatrix4x4::Initialize(const SFloat array[16])
	{
		this->Mat[0][0] = array[0];
		this->Mat[0][1] = array[1];
		this->Mat[0][2] = array[2];
		this->Mat[0][3] = array[3];
		this->Mat[1][0] = array[4];
		this->Mat[1][1] = array[5];
		this->Mat[1][2] = array[6];
		this->Mat[1][3] = array[7];
		this->Mat[2][0] = array[8];
		this->Mat[2][1] = array[9];
		this->Mat[2][2] = array[10];
		this->Mat[2][3] = array[11];
		this->Mat[3][0] = array[12];
		this->Mat[3][1] = array[13];
		this->Mat[3][2] = array[14];
		this->Mat[3][3] = array[15];
	}

	FMatrix4x4& FMatrix4x4::Multiply(const FMatrix4x4& other)
	{
		FMatrix4x4 TransposedOther = other;
		TransposedOther.Transpose();

		FMatrix4x4 Result;

		for (SUInt i = 0; i < 4; i++)
		{
			__m128 reg_a = _mm_load_ps(&this->Mat[i][0]);

			for (int j = 0; j < 4; ++j) 
			{
				__m128 reg_bt = _mm_load_ps(&TransposedOther[j][0]);

				__m128 reg_mul = _mm_mul_ps(reg_a, reg_bt);

				__m128 reg_sum1 = _mm_add_ps(reg_mul, _mm_movehl_ps(reg_mul, reg_mul));
				__m128 reg_sum2 = _mm_add_ss(reg_sum1, _mm_shuffle_ps(reg_sum1, reg_sum1, 0x1));

				_mm_store_ss(&Result[i][j], reg_sum2);
			}
		}

		*this = Result;

		return *this;
	}

	FMatrix4x4& FMatrix4x4::Identity()
	{
		this->Mat[0][0] = 1.0f;
		this->Mat[0][1] = 0.0f;
		this->Mat[0][2] = 0.0f;
		this->Mat[0][3] = 0.0f;
		this->Mat[1][0] = 0.0f;
		this->Mat[1][1] = 1.0f;
		this->Mat[1][2] = 0.0f;
		this->Mat[1][3] = 0.0f;
		this->Mat[2][0] = 0.0f;
		this->Mat[2][1] = 0.0f;
		this->Mat[2][2] = 1.0f;
		this->Mat[2][3] = 0.0f;
		this->Mat[3][0] = 0.0f;
		this->Mat[3][1] = 0.0f;
		this->Mat[3][2] = 0.0f;
		this->Mat[3][3] = 1.0f;

		return *this;
	}

	FMatrix4x4& FMatrix4x4::Transpose()
	{
		FMatrix4x4 transpose = {
			this->Mat[0][0],
			this->Mat[1][0],
			this->Mat[2][0],
			this->Mat[3][0],
			this->Mat[0][1],
			this->Mat[1][1],
			this->Mat[2][1],
			this->Mat[3][1],
			this->Mat[0][2],
			this->Mat[1][2],
			this->Mat[2][2],
			this->Mat[3][2],
			this->Mat[0][3],
			this->Mat[1][3],
			this->Mat[2][3],
			this->Mat[3][3] };

		*this = transpose;

		return *this;
	}

	FMatrix4x4 FMatrix4x4::operator*(const FMatrix4x4& other) const noexcept
	{
		FMatrix4x4 TransposedOther = other;
		TransposedOther.Transpose();

		FMatrix4x4 Result;

		for (SUInt i = 0; i < 4; i++)
		{
			__m128 reg_a = _mm_load_ps(&this->Mat[i][0]);

			for (int j = 0; j < 4; ++j)
			{
				__m128 reg_bt = _mm_load_ps(&TransposedOther[j][0]);

				__m128 reg_mul = _mm_mul_ps(reg_a, reg_bt);

				__m128 reg_sum1 = _mm_add_ps(reg_mul, _mm_movehl_ps(reg_mul, reg_mul));
				__m128 reg_sum2 = _mm_add_ss(reg_sum1, _mm_shuffle_ps(reg_sum1, reg_sum1, 0x1));

				_mm_store_ss(&Result[i][j], reg_sum2);
			}
		}

		return Result;
	}

	FMatrix4x4& FMatrix4x4::operator*=(const FMatrix4x4& other)
	{
		return this->Multiply(other);
	}

	const float* FMatrix4x4::operator[](SUInt index) const noexcept
	{
		assert(index < 4);

		return this->Mat[index];
	}

	float* FMatrix4x4::operator[](SUInt index)
	{
		assert(index < 4);

		return this->Mat[index];
	}
}

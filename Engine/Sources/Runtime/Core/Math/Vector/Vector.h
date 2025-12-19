#ifndef _SE_VECTOR_H_
#define _SE_VECTOR_H_
#include "../../Core.h"

namespace SE
{
	class SVector2
	{
	public:
		SVector2();
		SVector2(SFloat a);
		SVector2(SFloat x, SFloat y);
		SVector2(const SVector2& other);
		~SVector2() = default;

		SVector2 operator+(const SVector2& other);
		SVector2 operator-(const SVector2& other);
		SVector2 operator*(const SVector2& other);
		SVector2 operator*(SFloat a);
		SVector2 operator*(SInt a);

		SFloat& operator[](SUInt index);
		const SFloat& operator[](SUInt index) const;

		SVector2& operator+=(const SVector2& other);
		SVector2& operator-=(const SVector2& other);
		SVector2& operator*=(const SVector2& other);
		SVector2& operator*=(SFloat a);
		SVector2& operator*=(SInt a);

		bool operator==(const SVector2& other) const;
		bool operator!=(const SVector2& other) const;

		SFloat GetLength() const;

		float x, y;
	};
}

#endif
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

		SBool operator==(const SVector2& other) const;
		SBool operator!=(const SVector2& other) const;

		SFloat GetLength() const;
		SFloat Dot(const SVector2& other);

		SFloat x, y;
	};

	class SVector3
	{
	public:
		SVector3();
		SVector3(SFloat a);
		SVector3(SFloat x, SFloat y, SFloat z);
		SVector3(const SVector3& other);
		~SVector3() = default;

		SVector3 operator+(const SVector3& other);
		SVector3 operator-(const SVector3& other);
		SVector3 operator*(const SVector3& other);
		SVector3 operator*(SFloat a);
		SVector3 operator*(SInt a);

		SFloat& operator[](SUInt index);
		const SFloat& operator[](SUInt index) const;

		SVector3& operator+=(const SVector3& other);
		SVector3& operator-=(const SVector3& other);
		SVector3& operator*=(const SVector3& other);
		SVector3& operator*=(SFloat a);
		SVector3& operator*=(SInt a);

		SBool operator==(const SVector3& other) const;
		SBool operator!=(const SVector3& other) const;

		SFloat GetLength() const;
		SFloat Dot(const SVector3& other);

		SFloat x, y, z;
	};

	class SVector4
	{
	public:
		SVector4();
		SVector4(SFloat a);
		SVector4(SFloat x, SFloat y, SFloat z, SFloat w);
		SVector4(const SVector4& other);
		~SVector4() = default;

		SVector4 operator+(const SVector4& other);
		SVector4 operator-(const SVector4& other);
		SVector4 operator*(const SVector4& other);
		SVector4 operator*(SFloat a);
		SVector4 operator*(SInt a);

		SFloat& operator[](SUInt index);
		const SFloat& operator[](SUInt index) const;

		SVector4& operator+=(const SVector4& other);
		SVector4& operator-=(const SVector4& other);
		SVector4& operator*=(const SVector4& other);
		SVector4& operator*=(SFloat a);
		SVector4& operator*=(SInt a);

		SBool operator==(const SVector4& other) const;
		SBool operator!=(const SVector4& other) const;

		SFloat GetLength() const;
		SFloat Dot(const SVector4& other);

		SFloat x, y, z, w;
	};
}

#endif
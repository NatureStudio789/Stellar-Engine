#include "Vector.h"

namespace SE
{
	SVector2::SVector2()
	{
		this->x = this->y = 0.0f;
	}

	SVector2::SVector2(SFloat a)
	{
		this->x = this->y = a;
	}

	SVector2::SVector2(SFloat x, SFloat y)
	{
		this->x = x;
		this->y = y;
	}

	SVector2::SVector2(const SVector2& other)
	{
		this->x = other.x;
		this->y = other.y;
	}

	SVector2 SVector2::operator+(const SVector2& other)
	{
		SVector2 result;

		SFloat This[4] = { this->x, this->y, 0.0f, 0.0f };
		SFloat Other[4] = { other.x, other.y, 0.0f, 0.0f };
		SFloat ResultF4[4];

		SUtil::AddSIMD(This, Other, ResultF4);

		result.x = ResultF4[0];
		result.y = ResultF4[1];

		return result;
	}

	SVector2 SVector2::operator-(const SVector2& other)
	{
		SVector2 result;

		SFloat This[4] = { this->x, this->y, 0.0f, 0.0f };
		SFloat Other[4] = { -other.x, -other.y, 0.0f, 0.0f };
		SFloat ResultF4[4];

		SUtil::AddSIMD(This, Other, ResultF4);

		result.x = ResultF4[0];
		result.y = ResultF4[1];

		return result;
	}

	SVector2 SVector2::operator*(const SVector2& other)
	{
		SVector2 result;

		SFloat This[4] = { this->x, this->y, 0.0f, 0.0f };
		SFloat Other[4] = { other.x, other.y, 0.0f, 0.0f };
		SFloat ResultF4[4];

		SUtil::MultiplySIMD(This, Other, ResultF4);

		result.x = ResultF4[0];
		result.y = ResultF4[1];

		return result;
	}

	SVector2 SVector2::operator*(SFloat a)
	{
		SVector2 result;

		SFloat This[4] = { this->x, this->y, 0.0f, 0.0f };
		SFloat Other[4] = { a, a, 0.0f, 0.0f };
		SFloat ResultF4[4];

		SUtil::MultiplySIMD(This, Other, ResultF4);

		result.x = ResultF4[0];
		result.y = ResultF4[1];

		return result;
	}

	SVector2 SVector2::operator*(SInt a)
	{
		SVector2 result;

		SFloat This[4] = { this->x, this->y, 0.0f, 0.0f };
		SFloat Other[4] = { (SFloat)a, (SFloat)a, 0.0f, 0.0f };
		SFloat ResultF4[4];

		SUtil::MultiplySIMD(This, Other, ResultF4);

		result.x = ResultF4[0];
		result.y = ResultF4[1];

		return result;
	}

	SFloat& SVector2::operator[](SUInt index)
	{
		assert(index < 2);

		switch (index)
		{
			case 0:
			{
				return this->x;
				break;
			}

			case 1:
			{
				return this->y;
				break;
			}

			// For avoiding warnings.
			default:
			{
				return this->y;
				break;
			}
		}
	}

	const SFloat& SVector2::operator[](SUInt index) const
	{
		assert(index < 2);

		switch (index)
		{
			case 0:
			{
				return this->x;
				break;
			}

			case 1:
			{
				return this->y;
				break;
			}

			// For avoiding warnings.
			default:
			{
				return this->y;
				break;
			}
		}
	}

	SVector2& SVector2::operator+=(const SVector2& other)
	{
		(*this) = (*this) + other;

		return *this;
	}

	SVector2& SVector2::operator-=(const SVector2& other)
	{
		(*this) = (*this) - other;

		return *this;
	}

	SVector2& SVector2::operator*=(const SVector2& other)
	{
		(*this) = (*this) * other;

		return *this;
	}

	SVector2& SVector2::operator*=(SFloat a)
	{
		(*this) = (*this) * a;

		return *this;
	}

	SVector2& SVector2::operator*=(SInt a)
	{
		(*this) = (*this) * a;

		return *this;
	}

	SBool SVector2::operator==(const SVector2& other) const
	{
		return this->x == other.x && this->y == other.y;
	}

	SBool SVector2::operator!=(const SVector2& other) const
	{
		return this->x != other.x || this->y != other.y;
	}

	SFloat SVector2::GetLength() const
	{
		return std::sqrt((this->x * this->x) + (this->y * this->y));
	}

	SFloat SVector2::Dot(const SVector2& other)
	{
		return this->x * other.x + this->y * other.y;
	}


	SVector3::SVector3()
	{
		this->x = this->y = this->z = 0.0f;
	}

	SVector3::SVector3(SFloat a)
	{
		this->x = this->y = this->z = a;
	}

	SVector3::SVector3(SFloat x, SFloat y, SFloat z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	SVector3::SVector3(const SVector3& other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
	}

	SVector3 SVector3::operator+(const SVector3& other)
	{
		SVector3 result;

		SFloat This[4] = { this->x, this->y, this->z, 0.0f };
		SFloat Other[4] = { other.x, other.y, other.z, 0.0f };
		SFloat ResultF4[4];

		SUtil::AddSIMD(This, Other, ResultF4);

		result.x = ResultF4[0];
		result.y = ResultF4[1];
		result.z = ResultF4[2];

		return result;
	}

	SVector3 SVector3::operator-(const SVector3& other)
	{
		SVector3 result;

		SFloat This[4] = { this->x, this->y, this->z, 0.0f };
		SFloat Other[4] = { -other.x, -other.y, -other.z, 0.0f };
		SFloat ResultF4[4];

		SUtil::AddSIMD(This, Other, ResultF4);

		result.x = ResultF4[0];
		result.y = ResultF4[1];
		result.z = ResultF4[2];

		return result;
	}

	SVector3 SVector3::operator*(const SVector3& other)
	{
		SVector3 result;

		SFloat This[4] = { this->x, this->y, this->z, 0.0f };
		SFloat Other[4] = { other.x, other.y, other.z, 0.0f };
		SFloat ResultF4[4];

		SUtil::MultiplySIMD(This, Other, ResultF4);

		result.x = ResultF4[0];
		result.y = ResultF4[1];
		result.z = ResultF4[2];

		return result;
	}

	SVector3 SVector3::operator*(SFloat a)
	{
		SVector3 result;

		SFloat This[4] = { this->x, this->y, this->z, 0.0f };
		SFloat Other[4] = { a, a, a, 0.0f };
		SFloat ResultF4[4];

		SUtil::MultiplySIMD(This, Other, ResultF4);

		result.x = ResultF4[0];
		result.y = ResultF4[1];
		result.z = ResultF4[2];

		return result;
	}

	SVector3 SVector3::operator*(SInt a)
	{
		SVector3 result;

		SFloat This[4] = { this->x, this->y, this->z, 0.0f };
		SFloat Other[4] = { (SFloat)a, (SFloat)a, (SFloat)a, 0.0f };
		SFloat ResultF4[4];

		SUtil::MultiplySIMD(This, Other, ResultF4);

		result.x = ResultF4[0];
		result.y = ResultF4[1];
		result.z = ResultF4[2];

		return result;
	}

	SFloat& SVector3::operator[](SUInt index)
	{
		assert(index < 3);

		switch (index)
		{
		case 0:
		{
			return this->x;
			break;
		}

		case 1:
		{
			return this->y;
			break;
		}

		case 2:
		{
			return this->z;
			break;
		}

		// For avoiding warnings.
		default:
		{
			return this->y;
			break;
		}
		}
	}

	const SFloat& SVector3::operator[](SUInt index) const
	{
		assert(index < 2);

		switch (index)
		{
		case 0:
		{
			return this->x;
			break;
		}

		case 1:
		{
			return this->y;
			break;
		}

		case 2:
		{
			return this->z;
			break;
		}

		// For avoiding warnings.
		default:
		{
			return this->y;
			break;
		}
		}
	}

	SVector3& SVector3::operator+=(const SVector3& other)
	{
		(*this) = (*this) + other;

		return *this;
	}

	SVector3& SVector3::operator-=(const SVector3& other)
	{
		(*this) = (*this) - other;

		return *this;
	}

	SVector3& SVector3::operator*=(const SVector3& other)
	{
		(*this) = (*this) * other;

		return *this;
	}

	SVector3& SVector3::operator*=(SFloat a)
	{
		(*this) = (*this) * a;

		return *this;
	}

	SVector3& SVector3::operator*=(SInt a)
	{
		(*this) = (*this) * a;

		return *this;
	}

	SBool SVector3::operator==(const SVector3& other) const
	{
		return this->x == other.x && this->y == other.y && this->z == other.z;
	}

	SBool SVector3::operator!=(const SVector3& other) const
	{
		return this->x != other.x || this->y != other.y || this->z != other.z;
	}

	SFloat SVector3::GetLength() const
	{
		return std::sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
	}

	SFloat SVector3::Dot(const SVector3& other)
	{
		return this->x * other.x + this->y * other.y + this->z * other.z;
	}


	SVector4::SVector4()
	{
		this->x = this->y = this->z = this->w = 0.0f;
	}

	SVector4::SVector4(SFloat a)
	{
		this->x = this->y = this->z = this->w = a;
	}

	SVector4::SVector4(SFloat x, SFloat y, SFloat z, SFloat w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	SVector4::SVector4(const SVector4& other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		this->w = other.w;
	}

	SVector4 SVector4::operator+(const SVector4& other)
	{
		SVector4 result;

		SFloat This[4] = { this->x, this->y, this->z, this->w };
		SFloat Other[4] = { other.x, other.y, other.z, other.w };
		SFloat ResultF4[4];

		SUtil::AddSIMD(This, Other, ResultF4);

		result.x = ResultF4[0];
		result.y = ResultF4[1];
		result.z = ResultF4[2];
		result.w = ResultF4[3];

		return result;
	}

	SVector4 SVector4::operator-(const SVector4& other)
	{
		SVector4 result;

		SFloat This[4] = { this->x, this->y, this->z, this->w };
		SFloat Other[4] = { -other.x, -other.y, -other.z, -other.w };
		SFloat ResultF4[4];

		SUtil::AddSIMD(This, Other, ResultF4);

		result.x = ResultF4[0];
		result.y = ResultF4[1];
		result.z = ResultF4[2];
		result.w = ResultF4[3];

		return result;
	}

	SVector4 SVector4::operator*(const SVector4& other)
	{
		SVector4 result;

		SFloat This[4] = { this->x, this->y, this->z, this->w };
		SFloat Other[4] = { other.x, other.y, other.z, other.w };
		SFloat ResultF4[4];

		SUtil::MultiplySIMD(This, Other, ResultF4);

		result.x = ResultF4[0];
		result.y = ResultF4[1];
		result.z = ResultF4[2];
		result.w = ResultF4[3];

		return result;
	}

	SVector4 SVector4::operator*(SFloat a)
	{
		SVector4 result;

		SFloat This[4] = { this->x, this->y, this->z, this->w };
		SFloat Other[4] = { a, a, a, a };
		SFloat ResultF4[4];

		SUtil::MultiplySIMD(This, Other, ResultF4);

		result.x = ResultF4[0];
		result.y = ResultF4[1];
		result.z = ResultF4[2];
		result.w = ResultF4[3];

		return result;
	}

	SVector4 SVector4::operator*(SInt a)
	{
		SVector4 result;

		SFloat This[4] = { this->x, this->y, this->z, this->w };
		SFloat Other[4] = { (SFloat)a, (SFloat)a, (SFloat)a, (SFloat)a };
		SFloat ResultF4[4];

		SUtil::MultiplySIMD(This, Other, ResultF4);

		result.x = ResultF4[0];
		result.y = ResultF4[1];
		result.z = ResultF4[2];
		result.w = ResultF4[3];

		return result;
	}

	SFloat& SVector4::operator[](SUInt index)
	{
		assert(index < 4);

		switch (index)
		{
		case 0:
		{
			return this->x;
			break;
		}

		case 1:
		{
			return this->y;
			break;
		}

		case 2:
		{
			return this->z;
			break;
		}

		case 3:
		{
			return this->w;
			break;
		}

		// For avoiding warnings.
		default:
		{
			return this->y;
			break;
		}
		}
	}

	const SFloat& SVector4::operator[](SUInt index) const
	{
		assert(index < 2);

		switch (index)
		{
		case 0:
		{
			return this->x;
			break;
		}

		case 1:
		{
			return this->y;
			break;
		}

		case 2:
		{
			return this->z;
			break;
		}

		case 3:
		{
			return this->w;
			break;
		}

		// For avoiding warnings.
		default:
		{
			return this->y;
			break;
		}
		}
	}

	SVector4& SVector4::operator+=(const SVector4& other)
	{
		(*this) = (*this) + other;

		return *this;
	}

	SVector4& SVector4::operator-=(const SVector4& other)
	{
		(*this) = (*this) - other;

		return *this;
	}

	SVector4& SVector4::operator*=(const SVector4& other)
	{
		(*this) = (*this) * other;

		return *this;
	}

	SVector4& SVector4::operator*=(SFloat a)
	{
		(*this) = (*this) * a;

		return *this;
	}

	SVector4& SVector4::operator*=(SInt a)
	{
		(*this) = (*this) * a;

		return *this;
	}

	SBool SVector4::operator==(const SVector4& other) const
	{
		return this->x == other.x && this->y == other.y && this->z == other.z && this->w == other.w;
	}

	SBool SVector4::operator!=(const SVector4& other) const
	{
		return this->x != other.x || this->y != other.y || this->z != other.z || this->w != other.w;
	}

	SFloat SVector4::GetLength() const
	{
		return std::sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z) + (this->w * this->w));
	}

	SFloat SVector4::Dot(const SVector4& other)
	{
		return this->x * other.x + this->y * other.y + this->z * other.z + this->w * other.w;
	}
}

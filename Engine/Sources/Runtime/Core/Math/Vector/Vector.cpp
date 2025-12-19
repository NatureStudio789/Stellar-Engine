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

	bool SVector2::operator==(const SVector2& other) const
	{
		return this->x == other.x && this->y == other.y;
	}

	bool SVector2::operator!=(const SVector2& other) const
	{
		return this->x != other.x && this->y != other.y;
	}

	SFloat SVector2::GetLength() const
	{
		return std::sqrt((this->x * this->x) + (this->y * this->y));
	}
}

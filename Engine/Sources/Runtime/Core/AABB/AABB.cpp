#include <Core.h>
#include "AABB.h"

namespace SE
{
	SAABB::SAABB()
	{
		this->Min = { FLT_MAX, FLT_MAX, FLT_MAX };
		this->Max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
	}

	SAABB::SAABB(const glm::vec3& min, const glm::vec3& max)
	{
		this->Min = min;
		this->Max = max;
	}

	SAABB::SAABB(const SAABB& other)
	{
		this->Max = other.Max;
		this->Min = other.Min;
	}

	SAABB::~SAABB()
	{

	}

	void SAABB::Transform(glm::mat4x4 matrix)
	{
		this->Max = matrix * glm::vec4(this->Max, 1.0f);
		this->Min = matrix * glm::vec4(this->Min, 1.0f);
	}
}

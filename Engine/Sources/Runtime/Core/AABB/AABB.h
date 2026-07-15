#ifndef _SE_AABB_H_
#define _SE_AABB_H_
#include "../Addressable/Addressable.h"

namespace SE
{
	class SAABB
	{
	public:
		SAABB();
		SAABB(const glm::vec3& min, const glm::vec3& max);
		SAABB(const SAABB& other);
		~SAABB();

		void Transform(glm::mat4x4 matrix);

		/* This member is for getting the SPECIFIC (means that parent won't be the same category of each AABB) parent of this AABB instance,
		especially for getting the entity parent of this to do cursor picking,
		which is able to be invalid if it doesn't need using. */
		SAddressable::Identifier ParentIdentifier;

		glm::vec3 Max;
		glm::vec3 Min;
	};
}

#endif
#ifndef _SE_APPLICABLE_H_
#define _SE_APPLICABLE_H_
#include "../ContextDependent/ContextDependent.h"

namespace SE
{
	class GRenderable;

	class GApplicable : public GContextDependent
	{
	public:
		GApplicable() = default;
		virtual ~GApplicable() = default;

		virtual void SetParent(const GRenderable& parent) {}
		virtual void Apply() {}
	};
}

#endif
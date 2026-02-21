#ifndef _SE_APPLICABLE_H_
#define _SE_APPLICABLE_H_
#include "../ContextDependent/ContextDependent.h"

namespace SE
{
	class GApplicable : public GContextDependent
	{
	public:
		GApplicable() = default;
		virtual ~GApplicable() = default;

		virtual void Apply() {}
	};
}

#endif
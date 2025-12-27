#ifndef _SE_MONITORUTIL_H_
#define _SE_MONITORUTIL_H_
#include "../../../Core/Math/Vector/Vector.h"

namespace SE
{
	class FMonitorUtil
	{
	public:
		static SVector2 GetMonitorResolution();
		static SInt GetMonitorResolutionX();
		static SInt GetMonitorResolutionY();
	};
}

#endif
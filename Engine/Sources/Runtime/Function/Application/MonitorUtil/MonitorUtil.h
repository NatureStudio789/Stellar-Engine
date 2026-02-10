#ifndef _SE_MONITORUTIL_H_
#define _SE_MONITORUTIL_H_

namespace SE
{
	class FMonitorUtil
	{
	public:
		static glm::uvec2 GetMonitorResolution();
		static int GetMonitorResolutionX();
		static int GetMonitorResolutionY();
	};
}

#endif
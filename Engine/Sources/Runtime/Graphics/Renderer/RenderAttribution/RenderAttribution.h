#ifndef _SE_RENDERATTRIBUTION_H_
#define _SE_RENDERATTRIBUTION_H_
#include "../../../Core/Addressable/Addressable.h"

namespace SE
{
	class GRenderAttribution
	{
	public:
		GRenderAttribution();
		GRenderAttribution(const GRenderAttribution& other);
		~GRenderAttribution() = default;

		SUUID CommandListId;

		bool IsRequiringCamera;
		SUUID MainCameraId;
	};
}

#endif
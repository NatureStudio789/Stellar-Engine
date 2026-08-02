#include <Core.h>
#include "RenderAttribution.h"

namespace SE
{
	GRenderAttribution::GRenderAttribution()
	{
		this->IsRequiringCamera = false;
	}

	GRenderAttribution::GRenderAttribution(const GRenderAttribution& other)
	{
		this->CommandListId = other.CommandListId;

		this->IsRequiringCamera = other.IsRequiringCamera;
		this->MainCameraId = other.MainCameraId;
	}
}

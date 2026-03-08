#include <Core.h>
#include "RenderGroup.h"

namespace SE
{
	std::string GRenderGroup::LIGHTING_GROUP = "Lighting_RenderGroup";

	std::string GRenderGroup::WIREFRAME_GROUP = "Wireframe_RenderGroup";

	std::string GRenderGroup::FULLSCREEN_GROUP = "Fullscren_RenderGroup";

	std::string GRenderGroup::DEPTH_GROUP = "Depth_RenderGroup";
	
	bool GRenderGroup::CheckAvailable(const std::string& groupName)
	{
		return groupName == LIGHTING_GROUP || groupName == WIREFRAME_GROUP || 
			groupName == FULLSCREEN_GROUP || groupName == DEPTH_GROUP;
	}
}

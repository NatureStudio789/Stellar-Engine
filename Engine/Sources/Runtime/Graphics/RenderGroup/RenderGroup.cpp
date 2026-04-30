#include <Core.h>
#include "RenderGroup.h"

namespace SE
{
	std::string GRenderGroup::LIGHTING_GROUP = "Lighting_RenderGroup";

	std::string GRenderGroup::WIREFRAME_GROUP = "Wireframe_RenderGroup";

	std::string GRenderGroup::ALBEDO_GROUP = "Albedo_RenderGroup";
	std::string GRenderGroup::METALLIC_GROUP = "Metallic_RenderGroup";
	std::string GRenderGroup::ROUGHNESS_GROUP = "Roughness_RenderGroup";
	std::string GRenderGroup::NORMAL_GROUP = "Normal_RenderGroup";
	std::string GRenderGroup::COMPOSITION_GROUP = "Composition_RenderGroup";

	std::string GRenderGroup::FULLSCREEN_GROUP = "Fullscren_RenderGroup";

	std::string GRenderGroup::DEPTH_GROUP = "Depth_RenderGroup";
	
	bool GRenderGroup::CheckAvailable(const std::string& groupName)
	{
		return groupName == LIGHTING_GROUP || groupName == WIREFRAME_GROUP || groupName == ALBEDO_GROUP || 
			groupName == METALLIC_GROUP || groupName == ROUGHNESS_GROUP || groupName == NORMAL_GROUP || 
			groupName == COMPOSITION_GROUP || groupName == FULLSCREEN_GROUP || groupName == DEPTH_GROUP;
	}
}

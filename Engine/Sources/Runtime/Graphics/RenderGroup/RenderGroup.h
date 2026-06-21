#ifndef _SE_RENDERGROUP_H_
#define _SE_RENDERGROUP_H_
#include <string>

namespace SE
{
	/*  This registry class is for declaring the defferent categories of rendering.
		It is mainly used for pipeline state registration. */
	class GRenderGroup
	{
	public:
		static std::string LIGHTING_GROUP;

		static std::string WIREFRAME_GROUP;

		static std::string ALBEDO_GROUP;
		static std::string METALLIC_GROUP;
		static std::string ROUGHNESS_GROUP;
		static std::string NORMAL_GROUP;
		static std::string POSITION_GROUP;
		static std::string COMPOSITION_GROUP;

		static std::string FULLSCREEN_GROUP;

		static bool CheckAvailable(const std::string& groupName);
	};
}

#endif
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

		static std::string FULLSCREEN_GROUP;

		static std::string DEPTH_GROUP;

		static bool CheckAvailable(const std::string& groupName);
	};
}

#endif
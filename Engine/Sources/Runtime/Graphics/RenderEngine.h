#ifndef _SE_RENDERENGINE_H_
#define _SE_RENDERENGINE_H_
#include "../Core/EngineSystem/Engine.h"
#include "RenderConfiguration/RenderConfiguration.h"
#include "Renderer/Renderable/Mesh/StaticMesh.h"
#include "Renderer/Lighting/PointLight/PointLightRegistry.h"
#include "Renderer/Lighting/DirectionalLight/DirectionalLightRegistry.h"

namespace SE
{
	class RenderEngine : public SEngine
	{
	public:
		RenderEngine();

		void Initialize() override;

		void Execute() override;

		void Release() override;

	private:
		std::shared_ptr<GGraphicsContext> MainGraphicsContext;

		//std::shared_ptr<GMeshItem> test;
		//std::shared_ptr<GMeshItem> test1;
		std::shared_ptr<GStaticMesh> testmesh;
		//std::shared_ptr<GStaticMesh> testmesh1;
		std::shared_ptr<GStaticMesh> testmesh2;
		STransform TestMeshTransform;
		std::shared_ptr<GCamera> TestCamera;
		std::shared_ptr<GPointLightRegistry> TestPLRegistry;
		std::shared_ptr<GDirectionalLightRegistry> TestDLRegistry;
	};
}

#endif
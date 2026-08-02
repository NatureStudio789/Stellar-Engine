#ifndef _SE_SHADOWPASS_H_
#define _SE_SHADOWPASS_H_
#include "../RenderQueuePass.h"

namespace SE
{
	class GDirectionalLightRegistry;

	class GShadowPass : public GRenderQueuePass
	{
	public:
		GShadowPass();
		GShadowPass(const std::string& name);
		GShadowPass(const GShadowPass& other);
		~GShadowPass() override;

		void Initialize(const std::string& name);
		void SetLightRegistry(std::shared_ptr<GDirectionalLightRegistry> registry);

		void Apply() override;
		void Execute() override;

	private:
		std::shared_ptr<GDirectionalLightRegistry> DirectionalLightRegistry;

		std::shared_ptr<GOutflow> DirectionalDepthMapListOutflow;
	};
}

#endif
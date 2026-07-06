#ifndef _SE_COMPOSITIONPASS_H_
#define _SE_COMPOSITIONPASS_H_
#include "../RenderQueuePass.h"

namespace SE
{
	class GRenderable;

	class GCompositionPass : public GRenderQueuePass
	{
	public:
		struct AttriCBufferData
		{
			glm::vec3 ViewPosition;
		};

	public:
		GCompositionPass();
		GCompositionPass(const std::string& name);
		GCompositionPass(const GCompositionPass& other);
		~GCompositionPass() override;

		void Initialize(const std::string& name);

		void Execute() override;

	private:
		void ExecuteSetup();

		GResourcePackage GBufferPackage;

		std::shared_ptr<GRenderable> FramebufferPresenter;
		std::shared_ptr<GConstantBuffer<AttriCBufferData>> AttributionCBuffer;

		bool IsSetup = false;
	};
}

#endif
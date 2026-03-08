#ifndef _SE_EDITORRENDERPASS_H_
#define _SE_EDITORRENDERPASS_H_
#include "../../../Runtime/Graphics/Renderer/RenderPass/RenderPass.h"

namespace SE
{
	class GTexture;

	class EEditorRenderPass : public GRenderPass
	{
	public:
		EEditorRenderPass(const std::string& name);

		void Execute() override;

	private:
		GResourcePackage FramebufferPackage;
		std::shared_ptr<GTexture> texture;
	};
}

#endif
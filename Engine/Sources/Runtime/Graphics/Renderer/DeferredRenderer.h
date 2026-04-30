#ifndef _SE_DEFERREDRENDERER_H_
#define _SE_DEFERREDRENDERER_H_
#include "RenderPass/DeferredRendering/AlbedoPass.h"
#include "RenderPass/ClearPass.h"
#include "Renderer.h"

namespace SE
{
	class GDeferredRenderer : public GRenderer
	{
	public:
		GDeferredRenderer(const std::string& name);



	private:
		std::shared_ptr<GFramebuffer> GBufferFramebuffer;
	};
}

#endif